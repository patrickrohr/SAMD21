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

    auto operator->()
    {
        return m_pObj->Access(*this);
    }

    auto operator*()
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
    mutable T* m_pObj;
    mutable ID m_identifier;
};

} // namespace SAMD
