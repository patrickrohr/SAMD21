/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "error.hpp"
#include <cstdint>
#include <new>

namespace SAMD
{

template<typename T, unsigned SIZE>
class PreallocatedMemory
{
public:
    using value_type = T;
    using size_type  = unsigned;
    using index_type = unsigned;

private:
    static constexpr size_type g_uTotalSize = SIZE * sizeof(T);

public:
    PreallocatedMemory() : m_pMemory()
    {
    }

    T& operator[](size_type index)
    {
        // TODO: check boundaries
        return reinterpret_cast<T&>(m_pMemory[index * sizeof(T)]);
    }

    void Construct(index_type i, const T& value)
    {
        new (GetUnderlyingMemory(i)) T(value);
    }

    void Destroy(index_type i)
    {
        operator[](i).~T();
    }

    void* GetUnderlyingMemory(size_type index)
    {
        return static_cast<void*>(&operator[](index));
    }

private:
    uint8_t m_pMemory[g_uTotalSize];
};

} // namespace SAMD
