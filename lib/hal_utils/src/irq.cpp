/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "hal_utils/irq.hpp"
#include "common/error.hpp"

extern void __disable_irq();

namespace SAMD
{

template<Environment ENV>
int InterruptSafeGuardImpl<ENV>::m_depthCount = 0;

template<Environment ENV>
InterruptSafeGuardImpl<ENV>::InterruptSafeGuardImpl()
{
    if (m_depthCount++ == 0)
    {
        // __disable_irq();
    }
}

template<Environment ENV>
InterruptSafeGuardImpl<ENV>::~InterruptSafeGuardImpl()
{
    if (--m_depthCount == 0)
    {
        // __enable_irq();
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

#if BUILD_SIMULATION
template class InterruptSafeGuardImpl<Environment::eSimulation>;
#else
template class InterruptSafeGuardImpl<Environment::eTarget>;
#endif
} // namespace SAMD
