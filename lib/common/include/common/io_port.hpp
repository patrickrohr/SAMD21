// Copyright 2019, Patrick Rohr

#pragma once

#include "common/register.hpp"

namespace SAMD
{

template<typename T>
class IoPortBase
{
public:
    IoPortBase(volatile T* addr) : m_objRegister(addr)
    {
    }

    virtual ~IoPortBase() = default;

protected:
    register_t<T> m_objRegister;
};

template<typename T>
class IoPortRead : public IoPortBase<const T>
{
public:
    IoPortRead(const volatile T* addr) : IoPortBase<const T>(addr)
    {
    }

    ~IoPortRead() override = default;

    T Read() const
    {
        return this->m_objRegister;
    }

    volatile const T* operator->() const
    {
        return this->m_objRegister.Get();
    }
};

template<typename T>
class IoPortWrite : public IoPortBase<T>
{
public:
    IoPortWrite(volatile T* addr) : IoPortBase<T>(addr)
    {
    }

    ~IoPortWrite() override = default;

    void Write(const T& obj)
    {
        this->m_objRegister = obj;
    }

    volatile const T* operator->() const
    {
        return this->m_objRegister;
    }

    volatile T* operator->()
    {
        return this->m_objRegister.Get();
    }
};

template<typename T>
class IoPortRW : public IoPortBase<T>
{
public:
    IoPortRW(volatile T* addr) : IoPortBase<T>(addr)
    {
    }

    ~IoPortRW() override = default;

    T Read() const
    {
        return this->m_objRegister;
    }

    void Write(const T& obj)
    {
        this->m_objRegister = obj;
    }

    volatile const T* operator->() const
    {
        return this->m_objRegister.Get();
    }

    volatile T* operator->()
    {
        return this->m_objRegister.Get();
    }
};

// TODO: support registers that need to be written to before you can read them
// Take lambda in constructor or something.

} // namespace SAMD
