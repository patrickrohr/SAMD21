/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "iterator.hpp"

namespace SAMD
{

template<typename T, unsigned SIZE>
class Array
{
public:
    static constexpr unsigned SizeAllocated = SIZE;

    using value_type    = T;
    using index_type    = unsigned;
    using iterator_type = SequenceIterator<Array<T, SIZE>, index_type>;

public:
    Array() : m_arrData()
    {
    }

    T* Data()
    {
        return m_arrData[0];
    }

    T& operator[](const index_type& index)
    {
        samd_assert(index < SIZE, "Array Index Overflow");
        return m_arrData[index];
    }

    constexpr unsigned Size()
    {
        return SizeAllocated;
    }

    iterator_type Begin()
    {
        return iterator_type(this, 0);
    }

    iterator_type End()
    {
        return iterator_type(this, SIZE);
    }

private:
    // iterator support
    friend iterator_type;

    value_type* Access(const iterator_type& it)
    {
        return &operator[](it.m_identifier);
    }

    void Next(iterator_type& it)
    {
        ++it.m_identifier;
    }

    void Prev(iterator_type& it)
    {
        --it.m_identifier;
    }

private:
    T m_arrData[SIZE];
};

template<typename T, unsigned SIZE>
typename Array<T, SIZE>::iterator_type begin(Array<T, SIZE>& arr)
{
    return arr.Begin();
}

template<typename T, unsigned SIZE>
typename Array<T, SIZE>::iterator_type end(Array<T, SIZE>& arr)
{
    return arr.End();
}

} // namespace SAMD
