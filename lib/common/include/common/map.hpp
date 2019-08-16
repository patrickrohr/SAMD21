/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

namespace SAMD
{

template<typename K, typename V, unsigned SIZE>
class Map
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

    using iterator_type = Node*;
    using index_type    = unsigned;
    using size_type     = unsigned;

public:
    Map() : m_uSize(), m_arrData(), m_arrIsInitialized()
    {
    }

    bool Insert(const Node& newNode)
    {
        // Traverse tree, start at root = 0
        index_type index   = 0;
        bool isResultValid = FindImpl(newNode.key, index);

        if (isResultValid && !m_arrIsInitialized[index])
        {
            m_arrData[index]          = newNode;
            m_arrIsInitialized[index] = true;
            ++m_uSize;
            return true;
        }

        return false;
    }

    iterator_type Find(const K& key)
    {
        index_type index   = 0;
        bool isResultValid = FindImpl(key, index);

        if (isResultValid && m_arrIsInitialized[index])
        {
            return &m_arrData[index];
        }

        return End();
    }

    size_type Size()
    {
        return m_uSize;
    }

    iterator_type Begin()
    {
        // TODO
        return nullptr;
    }

    iterator_type End()
    {
        // TODO
        return nullptr;
    }

private:
    // could be uninitialized
    // returns false when index is out of range
    bool FindImpl(const K& key, index_type& index)
    {
        index = 0;

        while (m_arrIsInitialized[index])
        {
            bool isOkay = true;
            if (key == m_arrData[index].key)
            {
                return true;
            }
            else if (key < m_arrData[index].key)
            {
                isOkay = LeftChild(index);
            }
            else
            {
                isOkay = RightChild(index);
            }

            if (!isOkay)
            {
                // index overflow
                return false;
            }
        }

        // node not found but the index is still valid
        return true;
    }

    static bool LeftChild(index_type& index)
    {
        // 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10
        // 0, l0, r0, l1, r1, l2, r2, l3, r3, l4, r4
        // (i * 2) + 1
        index = (index * 2) + 1;

        return index < SIZE;
    }

    static bool RightChild(index_type& index)
    {
        // (i + 1) * 2
        index = (index + 1) * 2;

        return index < SIZE;
    }

private:
    unsigned m_uSize;
    Node m_arrData[SIZE];
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
