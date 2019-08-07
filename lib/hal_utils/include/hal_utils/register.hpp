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
struct register_traits_t
{
protected:
    using underlying_type          = typename fixed_width_int<sizeof(T)>::type;
    static constexpr unsigned size = sizeof(T);

protected:
    volatile underlying_type* GetPtr() volatile
    {
        return reinterpret_cast<volatile underlying_type*>(this);
    }

    const volatile underlying_type* GetPtr() const volatile
    {
        return reinterpret_cast<const volatile underlying_type*>(this);
    }
};

// IsShared parameter is only used for simulating shared register (registers
// which require an ID to be written before reading).
template<
    typename T,
    bool IsShared   = false,
    Environment ENV = eRuntimeEnvironment>
class RegisterGuard : register_traits_t<T>
{
public:
    RegisterGuard() = default;

    explicit RegisterGuard(const volatile RegisterGuard& rhs)
    {
        operator=(rhs);
    }

    void operator=(const volatile RegisterGuard& rhs) volatile
    {
        RegisterCopy(
            register_traits_t<T>::GetPtr(),
            rhs.GetPtr(),
            register_traits_t<T>::size);
    }

    const volatile T& Get() const volatile
    {
        return data;
    }

    volatile T& Get() volatile
    {
        return data;
    }

    const T& Get() const
    {
        return data;
    }

    T& Get()
    {
        return data;
    }

private:
    T data;

public:
    // delete copy constructors
    RegisterGuard(const RegisterGuard& rhs) = delete;
    RegisterGuard& operator=(const RegisterGuard& rhs) = delete;
};

template<typename T>
class RegisterGuard<T, true, Environment::eSimulation> : register_traits_t<T>
{
public:
    RegisterGuard() : data(), m_arrData()
    {
    }

    const volatile T& Get() const volatile
    {
        // TODO: I am not sure if this is going to work everywhere
        uint8_t id = static_cast<uint8_t>(data);

        // clear out last 8 bits
        // TODO: Make sure this agrees with endianness
        m_arrData[id] &= ~0xFF;
        m_arrData[id] |= id;

        // Before returning the data, we need to copy it from
        // m_arrData to data
        return m_arrData[id];
    }

    volatile T& Get() volatile
    {
        return const_cast<volatile T&>(
            const_cast<const volatile RegisterGuard*>(this)->Get());
    }

    const T& Get() const
    {
        return const_cast<const T&>(
            const_cast<const volatile RegisterGuard*>(this)->Get());
    }

    T& Get()
    {
        return const_cast<T&>(
            const_cast<const volatile RegisterGuard*>(this)->Get());
    }

private:
    // This is only going to hold the id
    uint8_t data;

private:
    T m_arrData[256];
};

/**
 * @brief      Helper template struct to work around missing partial function
 * template specialization support for MakeRegisterGuard.
 *
 * @tparam     T     Register Type
 */
template<typename T, bool IsShared, Environment ENV = eRuntimeEnvironment>
struct RegisterGuardHelper
{
    static volatile RegisterGuard<T, IsShared>* FromPointer(volatile T* pReg)
    {
        return reinterpret_cast<volatile RegisterGuard<T, IsShared>*>(pReg);
    }
};

template<typename T, bool IsShared>
struct RegisterGuardHelper<T, IsShared, Environment::eSimulation>
{
    static volatile RegisterGuard<T, IsShared, Environment::eSimulation>* FromPointer(volatile T* pReg)
    {
        static volatile RegisterGuard<T, IsShared, Environment::eSimulation> data;
        return &data;
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
volatile RegisterGuard<T, false, ENV>* MakeRegisterGuard(volatile T* pReg)
{
    return RegisterGuardHelper<T, false, ENV>::FromPointer(pReg);
}

template<typename T, Environment ENV = eRuntimeEnvironment>
volatile RegisterGuard<T, true, ENV>* MakeSharedRegisterGuard(volatile T* pReg)
{
    return RegisterGuardHelper<T, true, ENV>::FromPointer(pReg);
}

} // namespace SAMD
