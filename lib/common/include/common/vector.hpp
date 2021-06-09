/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "common/allocator.hpp"
#include "common/array.hpp"
#include "common/iterator.hpp"

namespace SAMD
{

template<typename T, unsigned SIZE>
class Vector
{
public:
    static constexpr unsigned SizeAllocated = SIZE;

    using value_type    = T;
    using iterator_type = SequenceIterator<Vector<T, SIZE>, unsigned>;
    using const_iterator_type =
        SequenceIterator<const Vector<T, SIZE>, unsigned>;
    using index_type = unsigned;

public:
    Vector() : m_objMemory(), m_uSizeUsed()
    {
    }

    T* Data()
    {
        return m_objMemory[0];
    }

    T& operator[](const index_type& index)
    {
        samd_assert(index < m_uSizeUsed, "Vector Index Overflow");
        return m_objMemory[index];
    }

    const T& operator[](const index_type& index) const
    {
        return const_cast<Vector<T, SIZE>*>(this)->operator[](index);
    }

    void PushBack(const T& obj)
    {
        samd_assert(m_uSizeUsed < SIZE, "Vector full.");

        m_objMemory.Construct(m_uSizeUsed++, obj);
    }

    void PopBack()
    {
        samd_assert(m_uSizeUsed > 0, "Vector empty.");

        if (m_uSizeUsed > 0)
        {
            m_objMemory.Destroy(--m_uSizeUsed);
        }
    }

    unsigned Size() const
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

    const_iterator_type ConstEnd() const
    {
        return const_iterator_type(this, m_uSizeUsed);
    }

private:
    // iterator support
    friend iterator_type;
    friend const_iterator_type;

    value_type* Access(const iterator_type& it)
    {
        return &operator[](it.m_identifier);
    }

    const value_type* Access(const const_iterator_type& it) const
    {
        return &operator[](it.m_identifier);
    }

    void Next(iterator_type& it)
    {
        ++it.m_identifier;
    }

    void Next(const const_iterator_type& it) const
    {
        ++it.m_identifier;
    }

    void Prev(iterator_type& it)
    {
        --it.m_identifier;
    }

private:
    PreallocatedMemory<T, SIZE> m_objMemory;
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
typename Vector<T, SIZE>::const_iterator_type begin(const Vector<T, SIZE>& vec)
{
    return vec.ConstBegin();
}

template<typename T, unsigned SIZE>
typename Vector<T, SIZE>::const_iterator_type end(const Vector<T, SIZE>& vec)
{
    return vec.ConstEnd();
}

} // namespace SAMD
