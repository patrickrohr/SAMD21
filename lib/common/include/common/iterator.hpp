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

template<typename T, typename ID>
class SequenceIterator
{
    friend T;

public:
    SequenceIterator(T* obj, ID identifier) :
        m_pObj(obj), m_identifier(identifier)
    {
    }

    SequenceIterator(const SequenceIterator&) = default;
    SequenceIterator& operator=(const SequenceIterator&) = default;

    bool operator==(const SequenceIterator& rhs) const
    {
        return m_pObj == rhs.m_pObj && m_identifier == rhs.m_identifier;
    }

    bool operator!=(const SequenceIterator& rhs) const
    {
        return !operator==(rhs);
    }

    const typename T::value_type* operator->() const
    {
        return m_pObj->Access(*this);
    }

    const typename T::value_type& operator*() const
    {
        return *operator->();
    }

    typename T::value_type* operator->()
    {
        return m_pObj->Access(*this);
    }

    typename T::value_type& operator*()
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
    ID m_identifier;
};

} // namespace SAMD
