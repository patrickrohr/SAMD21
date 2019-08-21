/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "common/error.hpp"

namespace SAMD
{

template<typename T>
class SequenceIterator
{
    friend T;

public:
    SequenceIterator(T* obj, unsigned index) : m_pObj(obj), m_index(index)
    {
    }

    SequenceIterator(const SequenceIterator&) = default;
    SequenceIterator& operator=(const SequenceIterator&) = default;

    bool operator==(const SequenceIterator& rhs) const
    {
        return m_pObj == rhs.m_pObj && m_index == rhs.m_index;
    }

    bool operator!=(const SequenceIterator& rhs) const
    {
        return !operator==(rhs);
    }

    const typename T::element_type* operator->() const
    {
        return &(*m_pObj)[m_index];
    }

    const typename T::element_type& operator*() const
    {
        return *operator->();
    }

    SequenceIterator& operator++()
    {
        // pre increment
        m_pObj->Next(*this);
        return *this;
    }

    SequenceIterator operator++(int)
    {
        // post increment
        SequenceIterator originalIterator = *this;
        this->operator++();
        return originalIterator;
    }

    SequenceIterator& operator--()
    {
        m_pObj->Prev(*this);
        return *this;
    }

    SequenceIterator operator--(int)
    {
        SequenceIterator originalIterator = *this;
        this->operator--();
        return originalIterator;
    }

private:
    T* m_pObj;
    unsigned m_index;
};

} // namespace SAMD
