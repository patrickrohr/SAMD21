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

static constexpr frequency_t g_uTargetFrequency = 48000000;

template<typename CONFIG>
DFLL48M<CONFIG>::DFLL48M(gclk_id_t id, const ClockSourceGeneric& sourceClock) :
    ClockSourceGeneric(id),
    CONFIG(),
    m_ioSysctrlDfllMultiplier(&SYSCTRL->DFLLMUL),
    m_ioSysctrlDfllControl(&SYSCTRL->DFLLCTRL),
    m_ioSysctrlPclksr(&SYSCTRL->PCLKSR),
    m_objSourceClock(sourceClock),
    m_uTargetFrequency(g_uTargetFrequency),
    m_eMode(Mode::eOpenLoop)
{
    // HACK: Do this properly.
    // Flash wait states to support 48MHz
    NVMCTRL->CTRLB.bit.RWS = 1;

    // Errata 9905
    SYSCTRL->DFLLCTRL.bit.ONDEMAND = 0;
    while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {}
}

template<typename CONFIG>
void DFLL48M<CONFIG>::SetMode(Mode eMode)
{
    m_eMode = eMode;
}

template<typename CONFIG>
void DFLL48M<CONFIG>::SetTargetFrequency(frequency_t targetFrequency)
{
    // TODO: Come up with a sane number
    samd_assert(
        targetFrequency > 1000,
        "Requested target frequency too low: %u",
        targetFrequency);
    m_uTargetFrequency = targetFrequency;
}

template<typename CONFIG>
error_t DFLL48M<CONFIG>::StartImpl()
{
    // Calculate the multiplier
    // Frequency is guaranteed to never be 0.
    unsigned uMultiplier = m_uTargetFrequency / m_objSourceClock.GetFrequency();

    SYSCTRL_DFLLMUL_Type dfllMultiplier;
    dfllMultiplier.bit.MUL = uMultiplier;

    // In Closed loop, we need to set the Coarse and Fine Adjustment steps
    if (m_eMode == Mode::eClosedLoop)
    {
        dfllMultiplier.bit.CSTEP = CONFIG::MultiplierCoarseStep;
        dfllMultiplier.bit.FSTEP = CONFIG::MultiplierFineStep;
    }

    m_ioSysctrlDfllMultiplier.Write(dfllMultiplier);
    WaitReady();

    // Set up DFLL Control
    SYSCTRL_DFLLCTRL_Type objDfllCtrlTmp;

    // Open / Closed Loop
    objDfllCtrlTmp.bit.RUNSTDBY = CONFIG::RunStandby;
    objDfllCtrlTmp.bit.ONDEMAND = CONFIG::OnDemand;

    if (m_eMode == Mode::eClosedLoop)
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
    WaitReady();

    // Enable
    objDfllCtrlTmp.bit.ENABLE = 1;
    m_ioSysctrlDfllControl.Write(objDfllCtrlTmp);

    // Wait for locks in closed loop
    if (m_eMode == Mode::eClosedLoop)
    {
        // TODO: think about how we could possibly time out here and return an
        // error
        // clang-format off
        while (!m_ioSysctrlPclksr->bit.DFLLLCKC &&
               !m_ioSysctrlPclksr->bit.DFLLLCKF);
        // clang-format on
    }

    // Finally, wait for clock to become ready
    WaitReady();

    return 0;
}

template<typename CONFIG>
error_t DFLL48M<CONFIG>::StopImpl()
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
    return m_uTargetFrequency;
}

template<typename CONFIG>
bool DFLL48M<CONFIG>::PollReady() const
{
    return m_ioSysctrlPclksr->bit.DFLLRDY;
}

template<typename CONFIG>
ClockType DFLL48M<CONFIG>::GetClockSourceType() const
{
    return ClockType::eDFLL48M;
}

} // namespace SAMD
