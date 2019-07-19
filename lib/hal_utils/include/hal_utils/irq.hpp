/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include <common/environment.hpp>
#include <common/error.hpp>
#include <samd21.h>

namespace SAMD
{

template<Environment ENV = eRuntimeEnvironment>
class InterruptSafeGuardImpl
{
public:
    InterruptSafeGuardImpl()
    {
        if (m_depthCount++ == 0)
        {
            __disable_irq();
        }
    }

    ~InterruptSafeGuardImpl()
    {
        if (--m_depthCount == 0)
        {
            __enable_irq();
        }
    }

private:
    static int m_depthCount;
};

template<>
InterruptSafeGuardImpl<Environment::eSimulation>::InterruptSafeGuardImpl()
{
    ++m_depthCount;
}

template<>
InterruptSafeGuardImpl<Environment::eSimulation>::~InterruptSafeGuardImpl()
{
    assert<eRuntimeConfiguration>(--m_depthCount >= 0);
}


using InterruptSafeGuard = InterruptSafeGuardImpl<>;

} // namespace SAMD
