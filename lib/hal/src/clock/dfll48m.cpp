/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "clock/dfll48m.hpp"
#include "config.h"

namespace SAMD
{

template<typename CONFIG>
DFLL48M<CONFIG>::DFLL48M(gclk_id_t id, const ClockSourceGeneric& sourceClock) :
    ClockSourceGeneric(id),
    CONFIG(),
    m_ioSysctrlDfllMultiplier(&SYSCTRL->DFLLMUL),
    m_ioSysctrlDfllControl(&SYSCTRL->DFLLCTRL),
    m_ioSysctrlPclksr(&SYSCTRL->PCLKSR),
    m_objSourceClock(sourceClock)
{
    // HACK: Do this properly.
    // Flash wait states to support 48MHz
    NVMCTRL->CTRLB.bit.RWS = 1;

    // Errata 9905
    SYSCTRL->DFLLCTRL.bit.ONDEMAND = 0;
    while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {}

    Start();
}

template<typename CONFIG>
DFLL48M<CONFIG>::~DFLL48M()
{
    Stop();
}

// This can probably be optimized quite a bit, since we have to sync registers 3 times
template<typename CONFIG>
error_t DFLL48M<CONFIG>::Start()
{
    // Calculate the multiplier
    // Frequency is guaranteed to never be 0.
    unsigned uMultiplier = CONFIG::TargetFrequency / m_objSourceClock.GetFrequency();

    SYSCTRL_DFLLMUL_Type dfllMultiplier;
    dfllMultiplier.bit.MUL = uMultiplier;

    // In Closed loop, we need to set the Coarse and Fine Adjustment steps
    if (CONFIG::Mode == DfllMode::eClosedLoop)
    {
        dfllMultiplier.bit.CSTEP = CONFIG::MultiplierCoarseStep;
        dfllMultiplier.bit.FSTEP = CONFIG::MultiplierFineStep;
    }

    m_ioSysctrlDfllMultiplier.Write(dfllMultiplier);
    RegisterSync();

    // Set up DFLL Control
    SYSCTRL_DFLLCTRL_Type objDfllCtrlTmp;

    // Open / Closed Loop
    objDfllCtrlTmp.bit.RUNSTDBY = CONFIG::RunStandby;
    objDfllCtrlTmp.bit.ONDEMAND = CONFIG::OnDemand;

    if (CONFIG::Mode == DfllMode::eClosedLoop)
    {
        objDfllCtrlTmp.bit.MODE     = CONFIG::CtrlMode;
        objDfllCtrlTmp.bit.WAITLOCK = CONFIG::CtrlWaitlock;
        objDfllCtrlTmp.bit.QLDIS    = CONFIG::CtrlQlDisable;
        objDfllCtrlTmp.bit.CCDIS    = CONFIG::CtrlCcDisable;
        objDfllCtrlTmp.bit.BPLCKC   = CONFIG::CtrlBplckc;
        objDfllCtrlTmp.bit.LLAW     = CONFIG::CtrlLlaw;
        objDfllCtrlTmp.bit.STABLE   = CONFIG::CtrlStable;
    }

    m_ioSysctrlDfllControl.Write(objDfllCtrlTmp);
    RegisterSync();

    // Enable
    objDfllCtrlTmp.bit.ENABLE = 1;
    m_ioSysctrlDfllControl.Write(objDfllCtrlTmp);

    // Wait for locks in closed loop
    if (CONFIG::Mode == DfllMode::eClosedLoop)
    {
        // TODO: think about how we could possibly time out here and return an
        // error
        // clang-format off
        while (!m_ioSysctrlPclksr->bit.DFLLLCKC &&
               !m_ioSysctrlPclksr->bit.DFLLLCKF);
        // clang-format on
    }

    // Finally, wait for clock to become ready
    RegisterSync();

    return 0;
}

template<typename CONFIG>
error_t DFLL48M<CONFIG>::Stop()
{
    // Disable
    SYSCTRL_DFLLCTRL_Type objDfllCtrlTmp;
    objDfllCtrlTmp.bit.ENABLE = 1;
    m_ioSysctrlDfllControl.Write(objDfllCtrlTmp);

    return 0;
}

template<typename CONFIG>
frequency_t DFLL48M<CONFIG>::GetFrequency() const
{
    return CONFIG::TargetFrequency;
}

template<typename CONFIG>
bool DFLL48M<CONFIG>::PollIsRunning() const
{
    return m_ioSysctrlPclksr->bit.DFLLRDY && m_ioSysctrlDfllControl->bit.ENABLE;
}

template<typename CONFIG>
ClockType DFLL48M<CONFIG>::GetClockSourceType() const
{
    return ClockType::eDFLL48M;
}

template<typename CONFIG>
error_t DFLL48M<CONFIG>::RegisterSync()
{
    while(!m_ioSysctrlPclksr->bit.DFLLRDY) {}
    return 0;
}

} // namespace SAMD
