/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "common/array.hpp"
#include "common/sequence_iterator.hpp"

namespace SAMD
{

template<typename T, unsigned SIZE>
class Vector
{
public:
    static constexpr unsigned SizeAllocated = SIZE;

    using element_type  = T;
    using iterator_type = SequenceIterator<Vector<T, SIZE>>;
    using const_iterator_type = SequenceIterator<const Vector<T, SIZE>>;
    using index_type    = unsigned;

private:
    friend iterator_type;

public:
    Vector() : m_arrData(), m_uSizeUsed()
    {
    }

    T* Data()
    {
        return m_arrData.Data();
    }

    T& operator[](const index_type& index)
    {
        samd_assert(index < m_uSizeUsed, "Vector Index Overflow");
        return m_arrData[index];
    }

    const T& operator[](const index_type& index) const
    {
        return const_cast<Vector<T, SIZE>*>(this)->operator[](index);
    }

    void Resize(unsigned newSize)
    {
        samd_assert(newSize < SIZE, "Resize failed.");

        if (m_uSizeUsed > newSize)
        {
            // de-allocate objects, is there a better option than replacing with
            // a default-constructed object?
            for (unsigned i = m_uSizeUsed; i > newSize; --i)
            { m_arrData[i] = T(); } }
        m_uSizeUsed = newSize;
    }

    void PushBack(const T& obj)
    {
        samd_assert(m_uSizeUsed < SIZE, "Vector full.");

        operator[](m_uSizeUsed++) = obj;
    }

    void PopBack()
    {
        samd_assert(m_uSizeUsed > 0, "Vector empty.");

        if (m_uSizeUsed > 0)
        {
            operator[](--m_uSizeUsed) = T();
        }
    }

    unsigned Size()
    {
        return m_uSizeUsed;
    }

    iterator_type Begin()
    {
        return iterator_type(this, 0);
    }

    const_iterator_type ConstBegin() const
    {
        return const_iterator_type(this, 0);
    }

    iterator_type End()
    {
        return iterator_type(this, m_uSizeUsed);
    }

    const_iterator_type ConstEnd()
    {
        return const_iterator_type(this, m_uSizeUsed);
    }

private:
    void Next(iterator_type& it)
    {
        ++it.m_index;
    }

    void Prev(iterator_type& it)
    {
        --it.m_index;
    }

private:
    Array<T, SIZE> m_arrData;
    unsigned m_uSizeUsed;
};

template<typename T, unsigned SIZE>
typename Vector<T, SIZE>::iterator_type begin(Vector<T, SIZE>& vec)
{
    return vec.Begin();
}

template<typename T, unsigned SIZE>
typename Vector<T, SIZE>::iterator_type end(Vector<T, SIZE>& vec)
{
    return vec.End();
}

template<typename T, unsigned SIZE>
typename Vector<T, SIZE>::iterator_type begin(const Vector<T, SIZE>& vec)
{
    return vec.ConstBegin();
}

template<typename T, unsigned SIZE>
typename Vector<T, SIZE>::iterator_type end(const Vector<T, SIZE>& vec)
{
    return vec.ConstEnd();
}

} // namespace SAMD
