/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

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
class IoPortRead : public virtual IoPortBase<T>
{
public:
    IoPortRead(volatile T* addr) : IoPortBase<T>(addr)
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
class IoPortWrite : public virtual IoPortBase<T>
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
class IoPortRW : public IoPortRead<T>, public IoPortWrite<T>
{
public:
    IoPortRW(volatile T* addr) : IoPortBase<T>(addr), IoPortRead<T>(addr), IoPortWrite<T>(addr)
    {
    }

    ~IoPortRW() override = default;

    using IoPortWrite<T>::operator->;
};

// TODO: support registers that need to be written to before you can read them
// Take lambda in constructor or something.

} // namespace SAMD
