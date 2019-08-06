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

template<typename T>
struct RegisterGuard
{
    using underlying_type          = typename fixed_width_int<sizeof(T)>::type;
    static constexpr unsigned size = sizeof(T);

    RegisterGuard() = default;

    explicit RegisterGuard(const volatile RegisterGuard& rhs)
    {
        operator=(rhs);
    }

    volatile RegisterGuard& operator=(
        const volatile RegisterGuard& rhs) volatile
    {
        RegisterCopy(
            reinterpret_cast<volatile underlying_type*>(this),
            reinterpret_cast<const volatile underlying_type*>(&rhs),
            size);
        return *this;
    }

    // delete copy constructors
    RegisterGuard(const RegisterGuard& rhs) = delete;
    RegisterGuard& operator=(const RegisterGuard& rhs) = delete;

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
    static volatile RegisterGuard<T>* FromPointer(volatile T* pReg)
    {
        return reinterpret_cast<volatile RegisterGuard<T>*>(pReg);
    }
};

template<typename T>
struct RegisterGuardHelper<T, Environment::eSimulation>
{
    static volatile RegisterGuard<T>* FromPointer(volatile T* pReg)
    {
        static T data;
        return reinterpret_cast<volatile RegisterGuard<T>*>(&data);
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
volatile RegisterGuard<T>* MakeRegisterGuard(volatile T* pReg)
{
    return RegisterGuardHelper<T, ENV>::FromPointer(pReg);
}

} // namespace SAMD
