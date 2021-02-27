/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "common/allocator.hpp"
#include "common/iterator.hpp"

namespace SAMD
{

template<typename K, typename V, unsigned SIZE>
class Map final
{
    static_assert(SIZE > 0, "Size must be larger than 0");

public:
    // TODO: replace with Pair
    struct Node
    {
        Node() : key(), value()
        {
        }

        Node(const K& key_, const V& value_) : key(key_), value(value_)
        {
        }

        K key;
        V value;
    };

    using iterator_type = SequenceIterator<Map<K, V, SIZE>, unsigned>;
    using size_type     = unsigned;
    using value_type    = Node;

public:
    static constexpr unsigned RootNode = 0;

    Map() :
        m_objMemory(),
        m_uSize(),
        m_itBegin(this, RootNode),
        m_itEnd(this, RootNode),
        m_arrIsInitialized()
    {
        // Could directly initialize m_itEnd but this is safer
        RightChildUnsafe(m_itEnd);
    }

    bool Insert(const Node& newNode)
    {
        // Traverse tree, start at root = 0
        iterator_type it(this, RootNode);
        bool result = FindImpl(newNode.key, it);

        if (!result)
        {
            // overflow
            return false;
        }

        if (!m_arrIsInitialized[it.m_identifier])
        {
            m_objMemory.Construct(it.m_identifier, newNode);
            m_arrIsInitialized[it.m_identifier] = true;

            // Update iterators
            if (m_uSize++ == 0)
            {
                m_itBegin = it;
                RightChildUnsafe(it);
                m_itEnd = it;
            }
            else if (m_itEnd == it)
            {
                RightChildUnsafe(it);
                m_itEnd = it;
            }
            else if (it->key < m_itBegin->key)
            {
                m_itBegin = it;
            }

            return true;
        }

        return false;
    }

    iterator_type Find(const K& key)
    {
        iterator_type it(this, RootNode);
        bool isResultValid = FindImpl(key, it);

        if (isResultValid && m_arrIsInitialized[it.m_identifier])
        {
            return it;
        }

        return m_itEnd;
    }

    size_type Size()
    {
        return m_uSize;
    }

    iterator_type Begin()
    {
        return m_itBegin;
    }

    iterator_type End()
    {
        return m_itEnd;
    }

private:
    // SequenceIterator support
    friend iterator_type;

    value_type* Access(const iterator_type& it)
    {
        return &operator[](it.m_identifier);
    }

    void Next(iterator_type& it)
    {
        // calling next on end iterator is undefined!

        if (RightChild(it))
        {
            while (LeftChild(it)) {}
            return;
        }

        while (!IsLeftChild(it) && !IsRoot(it)) { Parent(it); }

        bool isEnd = !Parent(it);

        if (isEnd)
        {
            it = m_itEnd;
        }
    }

    void Prev(iterator_type& it)
    {
        // calling prev on begin iterator is undefined!

        if (LeftChild(it))
        {
            while (RightChild(it)) {}
            return;
        }

        while (!IsRightChild(it) && !IsRoot(it)) { Parent(it); }
    }

    const value_type& operator[](unsigned index) const
    {
        return m_objMemory[index];
    }

    value_type& operator[](unsigned index)
    {
        return m_objMemory[index];
    }

private:
    // returns true if found, false if out of range.
    bool FindImpl(const K& key, iterator_type& it)
    {
        bool isInRange = true;
        while (isInRange && m_arrIsInitialized[it.m_identifier])
        {
            if (key == it->key)
            {
                return true;
            }
            else if (key < it->key)
            {
                isInRange = LeftChild(it, true);
            }
            else
            {
                isInRange = RightChild(it, true);
            }
        }

        // Node not found, but as long as the iterator is in range we are golden
        return isInRange;
    }

    static bool IsRoot(const iterator_type& it)
    {
        return it.m_identifier == 0;
    }

    static bool IsLeftChild(const iterator_type& it)
    {
        return it.m_identifier % 2;
    }

    static bool IsRightChild(const iterator_type& it)
    {
        return !IsLeftChild(it) && !IsRoot(it);
    }

    static bool Parent(iterator_type& it)
    {
        if (IsRoot(it))
        {
            return false;
        }

        it.m_identifier = (it.m_identifier - 1) / 2;
        return true;
    }

    bool UpdateIndex(iterator_type& it, unsigned newIndex, bool allowEmpty)
    {
        if (newIndex >= SIZE)
        {
            return false;
        }

        if (!allowEmpty && !m_arrIsInitialized[newIndex])
        {
            return false;
        }

        it.m_identifier = newIndex;
        return true;
    }

    bool LeftChild(iterator_type& it, bool allowEmpty = false)
    {
        auto newIndex = (it.m_identifier * 2) + 1;
        return UpdateIndex(it, newIndex, allowEmpty);
    }

    static void LeftChildUnsafe(iterator_type& it)
    {
        it.m_identifier = (it.m_identifier * 2) + 1;
    }

    bool RightChild(iterator_type& it, bool allowEmpty = false)
    {
        auto newIndex = (it.m_identifier + 1) * 2;
        return UpdateIndex(it, newIndex, allowEmpty);
    }

    static void RightChildUnsafe(iterator_type& it)
    {
        it.m_identifier = (it.m_identifier + 1) * 2;
    }

private:
    PreallocatedMemory<Node, SIZE> m_objMemory;
    unsigned m_uSize;
    iterator_type m_itBegin;
    iterator_type m_itEnd;
    bool m_arrIsInitialized[SIZE];
};

template<typename K, typename V, unsigned SIZE>
typename Map<K, V, SIZE>::iterator_type begin(Map<K, V, SIZE>& map)
{
    return map.Begin();
}

template<typename K, typename V, unsigned SIZE>
typename Map<K, V, SIZE>::iterator_type end(Map<K, V, SIZE>& map)
{
    return map.End();
}

} // namespace SAMD
