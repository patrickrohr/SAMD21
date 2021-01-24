/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "clock/samd/dfll48m.hpp"
#include "config.h"

namespace SAMD
{
// Options are configurable via kconfig tool
static constexpr unsigned gMultiplierCoarseStep = CONFIG_DFLL48M_MUL_CSTEP;
static constexpr unsigned gMultiplierFineStep   = CONFIG_DFLL48M_MUL_FSTEP;
static constexpr unsigned gRunStandby           = CONFIG_DFLL48M_CTRL_RUNSTDBY;
static constexpr unsigned gOnDemand             = CONFIG_DFLL48M_CTRL_ONDEMAND;
static constexpr unsigned gCtrlMode             = CONFIG_DFLL48M_CTRL_MODE;
static constexpr unsigned gCtrlWaitLock         = CONFIG_DFLL48M_CTRL_WAITLOCK;
static constexpr unsigned gCtrlQlDisable        = CONFIG_DFLL48M_CTRL_QLDIS;
static constexpr unsigned gCtrlCcDisable        = CONFIG_DFLL48M_CTRL_CCDIS;
static constexpr unsigned gCtrlBplckc           = CONFIG_DFLL48M_CTRL_BPLCKC;
static constexpr unsigned gCtrlLlaw             = CONFIG_DFLL48M_CTRL_LLAW;
static constexpr unsigned gCtrlStable           = CONFIG_DFLL48M_CTRL_STABLE;
static constexpr frequency_t gTargetFrequency   = 48000000;
static constexpr DfllMode gMode                 = DfllMode::eClosedLoop;

DFLL48M::DFLL48M(gclk_id_t id, const ClockBase& sourceClock) :
    GenericClock(id), m_objSourceClock(sourceClock)
{
    // HACK: Do this properly.
    // Flash wait states to support 48MHz
    NVMCTRL->CTRLB.bit.RWS = 1;

    // Errata 9905
    SYSCTRL->DFLLCTRL.bit.ONDEMAND = 0;
    while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {}

    Start();
}

DFLL48M::~DFLL48M()
{
    Stop();
}

// This can probably be optimized quite a bit, since we have to sync registers 3
// times
error_t DFLL48M::Start()
{
    // Calculate the multiplier
    // Frequency is guaranteed to never be 0.
    unsigned uMultiplier =
        gTargetFrequency / m_objSourceClock.GetOutputFrequency();

    SYSCTRL_DFLLMUL_Type tmp_DFLLMUL{};
    tmp_DFLLMUL.bit.MUL = uMultiplier;

    // In Closed loop, we need to set the Coarse and Fine Adjustment steps
    if (gMode == DfllMode::eClosedLoop)
    {
        tmp_DFLLMUL.bit.CSTEP = gMultiplierCoarseStep;
        tmp_DFLLMUL.bit.FSTEP = gMultiplierFineStep;
    }

    SYSCTRL->DFLLMUL.reg = tmp_DFLLMUL.reg;
    RegisterSync();

    // Set up DFLL Control
    SYSCTRL_DFLLCTRL_Type tmp_DFLLCTRL{};

    // Open / Closed Loop
    tmp_DFLLCTRL.bit.RUNSTDBY = gRunStandby;
    tmp_DFLLCTRL.bit.ONDEMAND = gOnDemand;

    if (gMode == DfllMode::eClosedLoop)
    {
        tmp_DFLLCTRL.bit.MODE     = gCtrlMode;
        tmp_DFLLCTRL.bit.WAITLOCK = gCtrlWaitLock;
        tmp_DFLLCTRL.bit.QLDIS    = gCtrlQlDisable;
        tmp_DFLLCTRL.bit.CCDIS    = gCtrlCcDisable;
        tmp_DFLLCTRL.bit.BPLCKC   = gCtrlBplckc;
        tmp_DFLLCTRL.bit.LLAW     = gCtrlLlaw;
        tmp_DFLLCTRL.bit.STABLE   = gCtrlStable;
    }

    SYSCTRL->DFLLCTRL.reg = tmp_DFLLCTRL.reg;
    RegisterSync();

    // Per datasheet, write enable bit separately
    tmp_DFLLCTRL.bit.ENABLE = 1;
    SYSCTRL->DFLLCTRL.reg   = tmp_DFLLCTRL.reg;

    // Wait for locks in closed loop
    if (gMode == DfllMode::eClosedLoop)
    {
        // TODO: think about how we could possibly time out here and return an
        // error
        // clang-format off
        while (!SYSCTRL->PCLKSR.bit.DFLLLCKC &&
               !SYSCTRL->PCLKSR.bit.DFLLLCKF);
        // clang-format on
    }

    // Finally, wait for clock to become ready
    RegisterSync();

    return 0;
}

error_t DFLL48M::Stop()
{
    // Disable
    SYSCTRL_DFLLCTRL_Type tmp_DFLLCTRL{};
    tmp_DFLLCTRL.bit.ENABLE = 1;
    SYSCTRL->DFLLCTRL.reg   = tmp_DFLLCTRL.reg;

    return 0;
}

frequency_t DFLL48M::GetFrequency() const
{
    // TODO: Check out gclk.c _gclk_get_dfll_input_frequency()
    return gTargetFrequency;
}

bool DFLL48M::PollIsRunning() const
{
    return SYSCTRL->PCLKSR.bit.DFLLRDY && SYSCTRL->DFLLCTRL.bit.ENABLE;
}

ClockType DFLL48M::GetClockSourceType() const
{
    return ClockType::eDFLL48M;
}

error_t DFLL48M::RegisterSync()
{
    while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {}
    return 0;
}

} // namespace SAMD
