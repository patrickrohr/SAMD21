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
    SequenceIterator(T& obj, unsigned index) : m_obj(obj), m_index(index)
    {
    }

    typename T::element_type& operator->()
    {
        return m_obj[m_index];
    }

    SequenceIterator& operator++()
    {
        // pre increment
        m_obj.Next(*this);
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
        m_obj.Prev(*this);
        return *this;
    }

    SequenceIterator operator--(int)
    {
        SequenceIterator originalIterator = *this;
        this->operator--();
        return originalIterator;
    }

private:
    T& m_obj;
    unsigned m_index;
};

} // namespace SAMD
