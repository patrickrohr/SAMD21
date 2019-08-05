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

static auto reg_SYSCTRL  = MakeRegisterGuard(SYSCTRL);
static auto reg_NVMCTRL  = MakeRegisterGuard(NVMCTRL);
static auto reg_DFLLMUL  = MakeRegisterGuard(&reg_SYSCTRL->data.DFLLMUL);
static auto reg_DFLLCTRL = MakeRegisterGuard(&reg_SYSCTRL->data.DFLLCTRL);
static auto reg_PCLKSR   = MakeRegisterGuard(&reg_SYSCTRL->data.PCLKSR);

template<typename CONFIG>
DFLL48M<CONFIG>::DFLL48M(gclk_id_t id, const ClockSourceGeneric& sourceClock) :
    ClockSourceGeneric(id),
    CONFIG(),
    m_objSourceClock(sourceClock)
{
    // HACK: Do this properly.
    // Flash wait states to support 48MHz
    reg_NVMCTRL->data.CTRLB.bit.RWS = 1;

    // Errata 9905
    reg_DFLLCTRL->data.bit.ONDEMAND = 0;
    while (!reg_PCLKSR->data.bit.DFLLRDY) {}

    Start();
}

template<typename CONFIG>
DFLL48M<CONFIG>::~DFLL48M()
{
    Stop();
}

// This can probably be optimized quite a bit, since we have to sync registers 3
// times
template<typename CONFIG>
error_t DFLL48M<CONFIG>::Start()
{
    // Calculate the multiplier
    // Frequency is guaranteed to never be 0.
    unsigned uMultiplier =
        CONFIG::TargetFrequency / m_objSourceClock.GetOutputFrequency();

    RegisterGuard<SYSCTRL_DFLLMUL_Type> tmp_DFLLMUL;
    tmp_DFLLMUL.data.bit.MUL = uMultiplier;

    // In Closed loop, we need to set the Coarse and Fine Adjustment steps
    if (CONFIG::Mode == DfllMode::eClosedLoop)
    {
        tmp_DFLLMUL.data.bit.CSTEP = CONFIG::MultiplierCoarseStep;
        tmp_DFLLMUL.data.bit.FSTEP = CONFIG::MultiplierFineStep;
    }

    *reg_DFLLMUL = tmp_DFLLMUL;
    RegisterSync();

    // Set up DFLL Control
    RegisterGuard<SYSCTRL_DFLLCTRL_Type> tmp_DFLLCTRL;

    // Open / Closed Loop
    tmp_DFLLCTRL.data.bit.RUNSTDBY = CONFIG::RunStandby;
    tmp_DFLLCTRL.data.bit.ONDEMAND = CONFIG::OnDemand;

    if (CONFIG::Mode == DfllMode::eClosedLoop)
    {
        tmp_DFLLCTRL.data.bit.MODE     = CONFIG::CtrlMode;
        tmp_DFLLCTRL.data.bit.WAITLOCK = CONFIG::CtrlWaitlock;
        tmp_DFLLCTRL.data.bit.QLDIS    = CONFIG::CtrlQlDisable;
        tmp_DFLLCTRL.data.bit.CCDIS    = CONFIG::CtrlCcDisable;
        tmp_DFLLCTRL.data.bit.BPLCKC   = CONFIG::CtrlBplckc;
        tmp_DFLLCTRL.data.bit.LLAW     = CONFIG::CtrlLlaw;
        tmp_DFLLCTRL.data.bit.STABLE   = CONFIG::CtrlStable;
    }

    *reg_DFLLCTRL = tmp_DFLLCTRL;
    RegisterSync();

    // Per data sheet, write enable bit separately
    tmp_DFLLCTRL.data.bit.ENABLE = 1;
    *reg_DFLLCTRL                = tmp_DFLLCTRL;

    // Wait for locks in closed loop
    if (CONFIG::Mode == DfllMode::eClosedLoop)
    {
        // TODO: think about how we could possibly time out here and return an
        // error
        // clang-format off
        while (!reg_PCLKSR->data.bit.DFLLLCKC &&
               !reg_PCLKSR->data.bit.DFLLLCKF);
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
    RegisterGuard<SYSCTRL_DFLLCTRL_Type> tmp_DFLLCTRL;
    tmp_DFLLCTRL.data.bit.ENABLE = 1;
    *reg_DFLLCTRL                = tmp_DFLLCTRL;

    return 0;
}

template<typename CONFIG>
frequency_t DFLL48M<CONFIG>::GetFrequency() const
{
    // TODO: Check out gclk.c _gclk_get_dfll_input_frequency()
    return CONFIG::TargetFrequency;
}

template<typename CONFIG>
bool DFLL48M<CONFIG>::PollIsRunning() const
{
    return reg_PCLKSR->data.bit.DFLLRDY && reg_DFLLCTRL->data.bit.ENABLE;
}

template<typename CONFIG>
ClockType DFLL48M<CONFIG>::GetClockSourceType() const
{
    return ClockType::eDFLL48M;
}

template<typename CONFIG>
error_t DFLL48M<CONFIG>::RegisterSync()
{
    while (!reg_PCLKSR->data.bit.DFLLRDY) {}
    return 0;
}

} // namespace SAMD
