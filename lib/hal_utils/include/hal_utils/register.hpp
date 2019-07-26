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

template<typename T, bool IsHardwareAddr = false>
struct RegisterGuard
{
    using underlying_type          = typename fixed_width_int<sizeof(T)>::type;
    static constexpr unsigned size = sizeof(T);

    RegisterGuard() = default;

    explicit RegisterGuard(
        const volatile RegisterGuard<T, !IsHardwareAddr>& rhs)
    {
        operator=(rhs);
    }

    volatile RegisterGuard& operator=(
        const volatile RegisterGuard<T, !IsHardwareAddr>& rhs) volatile
    {
        RegisterCopy(
            reinterpret_cast<volatile underlying_type*>(this),
            reinterpret_cast<const volatile underlying_type*>(&rhs),
            size);
        return *this;
    }

    // delete copy constructors
    RegisterGuard(const RegisterGuard<T, IsHardwareAddr>& rhs) = delete;
    RegisterGuard& operator=(const RegisterGuard<T, IsHardwareAddr>& rhs) =
        delete;

    volatile RegisterGuard<T, !IsHardwareAddr>& Convert()
    {
        return *reinterpret_cast<volatile RegisterGuard<T, !IsHardwareAddr>>(
            this);
    }

    T data;
};

/**
 * @brief      Helper template struct to work around missing partial function
 * template specialization support for MakeRegisterGuard.
 *
 * @tparam     T     Register Type
 */
template<typename T, Environment ENV = eRuntimeEnvironment>
struct RegisterGuardHelper
{
    static volatile RegisterGuard<T, true>* FromPointer(volatile T* pReg)
    {
        return reinterpret_cast<volatile RegisterGuard<T, true>*>(pReg);
    }
};

template<typename T>
struct RegisterGuardHelper<T, Environment::eSimulation>
{
    static volatile RegisterGuard<T, true>* FromPointer(volatile T* pReg)
    {
        static T data;
        return reinterpret_cast<volatile RegisterGuard<T, true>*>(&data);
    }
};

/**
 * @brief      Makes a register guard.
 * @details    Specifying this as a function template has the benefit that we
 * can make use of template argument deduction.
 * @param      pReg  The register pointer.
 *
 * @tparam     T     Register Type
 *
 * @return     Hardware Register Guard
 */
template<typename T, Environment ENV = eRuntimeEnvironment>
volatile RegisterGuard<T, true>* MakeRegisterGuard(volatile T* pReg)
{
    return RegisterGuardHelper<T, ENV>::FromPointer(pReg);
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
