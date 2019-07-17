/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "common/irq.hpp"
#include "common/error.hpp"

#include <samd21.h>

namespace SAMD
{

template<Environment ENV>
int InterruptSafeGuardImpl<ENV>::m_depthCount = 0;

template<Environment ENV>
InterruptSafeGuardImpl<ENV>::InterruptSafeGuardImpl()
{
    if (m_depthCount++ == 0)
    {
        __disable_irq();
    }
}

template<Environment ENV>
InterruptSafeGuardImpl<ENV>::~InterruptSafeGuardImpl()
{
    if (--m_depthCount == 0)
    {
        __enable_irq();
    }
}

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

template class InterruptSafeGuardImpl<Environment::eTarget>;
template class InterruptSafeGuardImpl<Environment::eSimulation>;
} // namespace SAMD
