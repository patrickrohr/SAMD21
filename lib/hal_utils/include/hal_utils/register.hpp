/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "common/environment.hpp"
#include "common/fixed_width_int.hpp"
#include "hal_utils/irq.hpp"

namespace SAMD
{

template<typename T>
inline void
RegisterCopy(volatile T* dest, const volatile T* source, unsigned size)
{
    (void)size;
    *dest = *source;
}

template<>
inline void RegisterCopy<char>(
    volatile char* dest,
    const volatile char* source,
    unsigned size)
{
    // Disable interrupts while reading wide register
    InterruptSafeGuard objGuard;
    for (unsigned i = 0; i < size; ++i) { dest[i] = source[i]; }
}

template<typename T, Environment ENV = eRuntimeEnvironment>
class register_t
{
    using underlying_type          = typename fixed_width_int<sizeof(T)>::type;
    static constexpr unsigned size = sizeof(T);

public:
    register_t(volatile T* addr) : m_pAddress(addr)
    {
    }

    register_t& operator=(const T& rhs)
    {
        RegisterCopy<underlying_type>(
            reinterpret_cast<volatile underlying_type*>(m_pAddress),
            reinterpret_cast<const underlying_type*>(&rhs),
            size);
        return *this;
    }

    operator T() const
    {
        underlying_type obj;
        RegisterCopy<underlying_type>(
            &obj,
            reinterpret_cast<const volatile underlying_type*>(m_pAddress),
            size);

        return *reinterpret_cast<T*>(&obj);
    }

    volatile T* Get()
    {
        return m_pAddress;
    }
    const volatile T* Get() const
    {
        return m_pAddress;
    }

private:
    volatile T* m_pAddress;
};

template<typename T>
class register_t<T, Environment::eSimulation>
{
public:
    register_t(volatile T* addr) : m_objRegisterSim()
    {
        (void)addr;
    }

    register_t& operator=(const T& rhs)
    {
        m_objRegisterSim = rhs;
        return *this;
    }

    operator T() const
    {
        return m_objRegisterSim;
    }

    volatile T* Get()
    {
        return &m_objRegisterSim;
    }
    const volatile T* Get() const
    {
        return &m_objRegisterSim;
    }

private:
    T m_objRegisterSim;
};

} // namespace SAMD
