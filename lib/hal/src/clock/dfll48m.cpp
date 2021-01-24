/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "clock/impl/dfll48m.hpp"
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

static auto reg_SYSCTRL  = MakeRegisterGuard(SYSCTRL);
static auto reg_NVMCTRL  = MakeRegisterGuard(NVMCTRL);
static auto reg_DFLLMUL  = MakeRegisterGuard(&reg_SYSCTRL->Get().DFLLMUL);
static auto reg_DFLLCTRL = MakeRegisterGuard(&reg_SYSCTRL->Get().DFLLCTRL);
static auto reg_PCLKSR   = MakeRegisterGuard(&reg_SYSCTRL->Get().PCLKSR);

DFLL48M::DFLL48M(gclk_id_t id, const ClockBase& sourceClock) :
    GenericClock(id), m_objSourceClock(sourceClock)
{
    // HACK: Do this properly.
    // Flash wait states to support 48MHz
    reg_NVMCTRL->Get().CTRLB.bit.RWS = 1;

    // Errata 9905
    reg_DFLLCTRL->Get().bit.ONDEMAND = 0;
    while (!reg_PCLKSR->Get().bit.DFLLRDY) {}

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

    RegisterGuard<SYSCTRL_DFLLMUL_Type> tmp_DFLLMUL;
    tmp_DFLLMUL.Get().bit.MUL = uMultiplier;

    // In Closed loop, we need to set the Coarse and Fine Adjustment steps
    if (gMode == DfllMode::eClosedLoop)
    {
        tmp_DFLLMUL.Get().bit.CSTEP = gMultiplierCoarseStep;
        tmp_DFLLMUL.Get().bit.FSTEP = gMultiplierFineStep;
    }

    *reg_DFLLMUL = tmp_DFLLMUL;
    RegisterSync();

    // Set up DFLL Control
    RegisterGuard<SYSCTRL_DFLLCTRL_Type> tmp_DFLLCTRL;

    // Open / Closed Loop
    tmp_DFLLCTRL.Get().bit.RUNSTDBY = gRunStandby;
    tmp_DFLLCTRL.Get().bit.ONDEMAND = gOnDemand;

    if (gMode == DfllMode::eClosedLoop)
    {
        tmp_DFLLCTRL.Get().bit.MODE     = gCtrlMode;
        tmp_DFLLCTRL.Get().bit.WAITLOCK = gCtrlWaitLock;
        tmp_DFLLCTRL.Get().bit.QLDIS    = gCtrlQlDisable;
        tmp_DFLLCTRL.Get().bit.CCDIS    = gCtrlCcDisable;
        tmp_DFLLCTRL.Get().bit.BPLCKC   = gCtrlBplckc;
        tmp_DFLLCTRL.Get().bit.LLAW     = gCtrlLlaw;
        tmp_DFLLCTRL.Get().bit.STABLE   = gCtrlStable;
    }

    *reg_DFLLCTRL = tmp_DFLLCTRL;
    RegisterSync();

    // Per Get() sheet, write enable bit separately
    tmp_DFLLCTRL.Get().bit.ENABLE = 1;
    *reg_DFLLCTRL                 = tmp_DFLLCTRL;

    // Wait for locks in closed loop
    if (gMode == DfllMode::eClosedLoop)
    {
        // TODO: think about how we could possibly time out here and return an
        // error
        // clang-format off
        while (!reg_PCLKSR->Get().bit.DFLLLCKC &&
               !reg_PCLKSR->Get().bit.DFLLLCKF);
        // clang-format on
    }

    // Finally, wait for clock to become ready
    RegisterSync();

    return 0;
}

error_t DFLL48M::Stop()
{
    // Disable
    RegisterGuard<SYSCTRL_DFLLCTRL_Type> tmp_DFLLCTRL;
    tmp_DFLLCTRL.Get().bit.ENABLE = 1;
    *reg_DFLLCTRL                 = tmp_DFLLCTRL;

    return 0;
}

frequency_t DFLL48M::GetFrequency() const
{
    // TODO: Check out gclk.c _gclk_get_dfll_input_frequency()
    return gTargetFrequency;
}

bool DFLL48M::PollIsRunning() const
{
    return reg_PCLKSR->Get().bit.DFLLRDY && reg_DFLLCTRL->Get().bit.ENABLE;
}

ClockType DFLL48M::GetClockSourceType() const
{
    return ClockType::eDFLL48M;
}

error_t DFLL48M::RegisterSync()
{
    while (!reg_PCLKSR->Get().bit.DFLLRDY) {}
    return 0;
}

} // namespace SAMD
