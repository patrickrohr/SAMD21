// Copyright 2021, Patrick Rohr

#pragma once

#include "common/allocator.hpp"
#include "common/array.hpp"
#include "common/iterator.hpp"
#include <cstdint>

namespace SAMD
{

// TODO: doubly linked list is a little more CPU efficient, but needs more
// memory. Consider changing to singly linked list.
template<typename T>
class CircularListItem final
{
public:
    CircularListItem() : m_prev(this), m_next(this), m_memory()
    {
    }

    // TODO: separate CircularListHeader from CircularListItem
    virtual ~CircularListItem() = default;

    /**
     * Returns true if this item is alone in the list
     */
    bool IsAlone()
    {
        return m_next == this;
    }

    CircularListItem& Next()
    {
        return *m_next;
    }

    CircularListItem& Prev()
    {
        return *m_prev;
    }

    T& GetData()
    {
        return m_memory[0];
    }

    void Construct(const T& value)
    {
        m_memory.Construct(0, value);
    }

    void Destroy()
    {
        m_memory.Destroy(0);
    }

    void RemoveFromCurrentList()
    {
        // remove item from its current list
        // item->prev -> item -> item->next ==> item->prev -> item->next
        m_prev->m_next = m_next;
        m_next->m_prev = m_prev;
    }

    void AppendItem(CircularListItem<T>& item)
    {
        // add to this list
        // this -> item -> next
        item.m_prev    = this;
        item.m_next    = m_next;
        m_next->m_prev = &item;
        m_next         = &item;
    }

    void PrependItem(CircularListItem<T>& item)
    {
        // prev -> item -> this
        item.m_prev    = m_prev;
        item.m_next    = this;
        m_prev->m_next = &item;
        m_prev         = &item;
    }

public:
    CircularListItem(const CircularListItem&) = delete;
    CircularListItem& operator=(const CircularListItem&) = delete;

private:
    CircularListItem<T>* m_prev;
    CircularListItem<T>* m_next;
    PreallocatedMemory<T, 1> m_memory;
};

template<typename T, unsigned SIZE>
class List
{
public:
    using value_type    = T;
    using size_type     = unsigned;
    using iterator_type = SequenceIterator<List<T, SIZE>, CircularListItem<T>*>;

public:
    List() : m_listItems(), m_usedListHead(), m_freeListHead(), m_size(0)
    {
        for (CircularListItem<T>& buffer : m_listItems)
        { m_freeListHead.AppendItem(buffer); }
    }

    void PushBack(const T& obj)
    {
        auto& listItem = AllocateItem(obj);
        m_usedListHead.PrependItem(listItem);
    }

    void PushFront(const T& obj)
    {
        auto& listItem = AllocateItem(obj);
        m_usedListHead.AppendItem(listItem);
    }

    iterator_type Erase(const iterator_type& it)
    {
        // returns iterator of next object
        auto& item = *it.m_identifier;
        auto& next = item.Next();

        item.RemoveFromCurrentList();
        FreeItem(item);

        return iterator_type(this, &next);
    }

    // insert value before iterator
    iterator_type Insert(const iterator_type& it, const T& value)
    {
        auto& listItem = AllocateItem(value);
        it.m_identifier->PrependItem(listItem);
        return iterator_type(this, &listItem);
    }

    iterator_type Begin()
    {
        return iterator_type(this, &m_usedListHead.Next());
    }

    iterator_type End()
    {
        return iterator_type(this, &m_usedListHead);
    }

    bool Empty()
    {
        return m_size == 0;
    }

    size_type Size()
    {
        return m_size;
    }

    size_type Available()
    {
        return SIZE - m_size;
    }

private:
    // this removes the item from the free list and constructs it.
    // AppendItem() or PrependItem() needs to be called after.
    CircularListItem<T>& AllocateItem(const T& obj)
    {
        if (m_freeListHead.IsAlone())
        {
            // TODO: error!
            while (true)
                ;
        }

        auto& listItem = m_freeListHead.Next();
        listItem.RemoveFromCurrentList();
        listItem.Construct(obj);
        ++m_size;
        return listItem;
    }

    // this adds the item to the free list and destructs it.
    // RemoveFromCurrentList() should be called before.
    void FreeItem(CircularListItem<T>& item)
    {
        // destructor should be called...
        item.Destroy();
        m_freeListHead.AppendItem(item);
        --m_size;
    }

private:
    // iterator support
    friend iterator_type;

    value_type* Access(const iterator_type& it)
    {
        return &it.m_identifier->GetData();
    }

    void Next(iterator_type& it)
    {
        it.m_identifier = &it.m_identifier->Next();
    }

    void Prev(iterator_type& it)
    {
        it.m_identifier = &it.m_identifier->Prev();
    }

private:
    Array<CircularListItem<T>, SIZE> m_listItems;
    CircularListItem<T> m_usedListHead;
    CircularListItem<T> m_freeListHead;
    size_type m_size;
};

template<typename T, unsigned SIZE>
typename List<T, SIZE>::iterator_type begin(List<T, SIZE>& list)
{
    return list.Begin();
}

template<typename T, unsigned SIZE>
typename List<T, SIZE>::iterator_type end(List<T, SIZE>& list)
{
    return list.End();
}

} // namespace SAMD
