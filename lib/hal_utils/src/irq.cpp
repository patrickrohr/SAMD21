/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "hal_utils/irq.hpp"
#include "common/error.hpp"

#if BUILD_TARGET
#include <samd21.h>
#endif

namespace SAMD
{

template<Environment ENV>
int InterruptSafeGuardImpl<ENV>::g_depthCount = 0;

template<Environment ENV>
InterruptSafeGuardImpl<ENV>::InterruptSafeGuardImpl()
{
    if (g_depthCount++ == 0)
    {
#if BUILD_TARGET
        __enable_irq();
#endif
    }
}

template<Environment ENV>
InterruptSafeGuardImpl<ENV>::~InterruptSafeGuardImpl()
{
    if (--g_depthCount == 0)
    {
#if BUILD_TARGET
        __disable_irq();
#endif
    }
}

template<>
InterruptSafeGuardImpl<Environment::eSimulation>::InterruptSafeGuardImpl()
{
    ++g_depthCount;
}

template<>
InterruptSafeGuardImpl<Environment::eSimulation>::~InterruptSafeGuardImpl()
{
    assert<eRuntimeConfiguration>(--g_depthCount >= 0);
}

template class InterruptSafeGuardImpl<Environment::eTarget>;
template class InterruptSafeGuardImpl<Environment::eSimulation>;

} // namespace SAMD
