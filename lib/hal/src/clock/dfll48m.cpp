/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "clock/dfll48m.hpp"
#include "common/error.hpp"
#include "config.h"

namespace SAMD
{

static constexpr frequency_t g_uTargetFrequency = 48000000;

DFLL48M::DFLL48M(const ClockSourceGeneric& sourceClock) :
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

void DFLL48M::SetMode(Mode eMode)
{
    m_eMode = eMode;
}

void DFLL48M::SetTargetFrequency(frequency_t targetFrequency)
{
    // TODO: Come up with a sane number
    assert<>(targetFrequency > 1000);
    m_uTargetFrequency = targetFrequency;
}

error_t DFLL48M::StartImpl()
{
    // Calculate the multiplier
    // Frequency is guaranteed to never be 0.
    unsigned uMultiplier = m_uTargetFrequency / m_objSourceClock.GetFrequency();

    SYSCTRL_DFLLMUL_Type dfllMultiplier;
    dfllMultiplier.bit.MUL = uMultiplier;

    // In Closed loop, we need to set the Coarse and Fine Adjustment steps
    if (m_eMode == Mode::eClosedLoop)
    {
        dfllMultiplier.bit.CSTEP = CONFIG_DFLL48M_MUL_CSTEP;
        dfllMultiplier.bit.FSTEP = CONFIG_DFLL48M_MUL_FSTEP;
    }

    m_ioSysctrlDfllMultiplier.Write(dfllMultiplier);
    WaitReady();

    // Set up DFLL Control
    SYSCTRL_DFLLCTRL_Type objDfllCtrlTmp;

    // Open / Closed Loop
    objDfllCtrlTmp.bit.RUNSTDBY = CONFIG_DFLL48M_CTRL_RUNSTDBY;
    objDfllCtrlTmp.bit.ONDEMAND = CONFIG_DFLL48M_CTRL_ONDEMAND;

    if (m_eMode == Mode::eClosedLoop)
    {
        objDfllCtrlTmp.bit.MODE     = CONFIG_DFLL48M_CTRL_MODE;
        objDfllCtrlTmp.bit.WAITLOCK = CONFIG_DFLL48M_CTRL_WAITLOCK;
        objDfllCtrlTmp.bit.QLDIS    = CONFIG_DFLL48M_CTRL_QLDIS;
        objDfllCtrlTmp.bit.CCDIS    = CONFIG_DFLL48M_CTRL_CCDIS;
        objDfllCtrlTmp.bit.BPLCKC   = CONFIG_DFLL48M_CTRL_BPLCKC;
        objDfllCtrlTmp.bit.LLAW     = CONFIG_DFLL48M_CTRL_LLAW;
        objDfllCtrlTmp.bit.STABLE   = CONFIG_DFLL48M_CTRL_STABLE;
    }

    m_ioSysctrlDfllControl.Write(objDfllCtrlTmp);
    WaitReady();

    // Enable
    objDfllCtrlTmp.bit.ENABLE = 1;
    m_ioSysctrlDfllControl.Write(objDfllCtrlTmp);

    // Wait for locks in closed loop
    if (m_eMode == Mode::eClosedLoop)
    {
        // TODO: think about how we could possibly time out here and return an error
        // clang-format off
        while (!m_ioSysctrlPclksr->bit.DFLLLCKC &&
               !m_ioSysctrlPclksr->bit.DFLLLCKF);
        // clang-format on
    }

    // Finally, wait for clock to become ready
    WaitReady();

    return 0;
}

error_t DFLL48M::StopImpl()
{
    // Disable
    SYSCTRL_DFLLCTRL_Type objDfllCtrlTmp;
    objDfllCtrlTmp.bit.ENABLE = 1;
    m_ioSysctrlDfllControl.Write(objDfllCtrlTmp);

    return 0;
}

frequency_t DFLL48M::GetFrequency() const
{
    return m_uTargetFrequency;
}

bool DFLL48M::PollReady() const
{
    return m_ioSysctrlPclksr->bit.DFLLRDY;
}

ClockType DFLL48M::GetClockSourceType() const
{
    return ClockType::eDFLL48M;
}

} // namespace SAMD
