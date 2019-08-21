/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "common/sequence_iterator.hpp"

namespace SAMD
{

template<typename K, typename V, unsigned SIZE>
class Map final
{
    static_assert(SIZE > 0, "Size must be larger than 0");

public:
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

    using index_type    = unsigned;
    using iterator_type = SequenceIterator<Map<K, V, SIZE>>;
    using size_type     = unsigned;
    using element_type  = Node;

public:
    Map() : m_arrData(), m_uSize(), m_itBegin(this, 0), m_itEnd(this, 0), m_arrIsInitialized()
    {
        // Could directly initialize m_itEnd but this is safer
        RightChildUnsafe(m_itEnd);
    }

    bool Insert(const Node& newNode)
    {
        // Traverse tree, start at root = 0
        iterator_type it(this, 0);
        bool result = FindImpl(newNode.key, it);

        if (!result)
        {
            // overflow
            return false;
        }

        if (!m_arrIsInitialized[it.m_index])
        {
            m_arrData[it.m_index]          = newNode;
            m_arrIsInitialized[it.m_index] = true;
            ++m_uSize;

            // Update iterators
            if (m_uSize == 1)
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
        iterator_type it(this, 0);
        bool isResultValid = FindImpl(key, it);

        if (isResultValid && m_arrIsInitialized[it.m_index])
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

    const element_type& operator[](unsigned index)
    {
        return m_arrData[index];
    }

private:
    // returns true if found, false if out of range.
    bool FindImpl(const K& key, iterator_type& it)
    {
        while (m_arrIsInitialized[it.m_index])
        {
            bool isInRange = true;
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

            if (!isInRange)
            {
                // index overflow
                return false;
            }
        }

        // node not found but the index is still valid
        return true;
    }

    static bool IsRoot(const iterator_type& it)
    {
        return it.m_index == 0;
    }

    static bool IsLeftChild(const iterator_type& it)
    {
        return it.m_index % 2;
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

        it.m_index = (it.m_index - 1) / 2;
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

        it.m_index = newIndex;
    }

    bool LeftChild(iterator_type& it, bool allowEmpty = false)
    {
        auto newIndex = (it.m_index * 2) + 1;
        return UpdateIndex(it, newIndex, allowEmpty);
    }

    // DANGEROUS
    static void LeftChildUnsafe(iterator_type& it)
    {
        it.m_index = (it.m_index * 2) + 1;
    }

    bool RightChild(iterator_type& it, bool allowEmpty = false)
    {
        auto newIndex = (it.m_index + 1) * 2;
        return UpdateIndex(it, newIndex, allowEmpty);
    }

    // DANGEROUS
    static void RightChildUnsafe(iterator_type& it)
    {
        it.m_index = (it.m_index + 1) * 2;
    }

private:
    Node m_arrData[SIZE];
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
