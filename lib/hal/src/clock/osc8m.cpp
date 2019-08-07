// Copyright 2019, Patrick Rohr

#include "clock/osc8m.hpp"
#include "config.h"

namespace SAMD
{

static auto reg_SYSCTRL = MakeRegisterGuard(SYSCTRL);
static auto reg_OSC8M = MakeRegisterGuard(&reg_SYSCTRL->Get().OSC8M);
static auto reg_PCLKSR = MakeRegisterGuard(&reg_SYSCTRL->Get().PCLKSR);

template<typename CONFIG>
OSC8M<CONFIG>::OSC8M(gclk_id_t id) :
    ClockSourceGeneric(id),
    CONFIG()
{
    Start();
}

template<typename CONFIG>
OSC8M<CONFIG>::~OSC8M() {
    Stop();
}

template<typename CONFIG>
error_t OSC8M<CONFIG>::Start()
{
    // Leave Factory Values for FRANGE and CALIB
    RegisterGuard<SYSCTRL_OSC8M_Type> tmp_OSC8M(*reg_OSC8M);

    tmp_OSC8M.Get().bit.ENABLE   = 1;
    tmp_OSC8M.Get().bit.PRESC    = CONFIG::Prescaler; // prescaler of 1
    tmp_OSC8M.Get().bit.ONDEMAND = CONFIG::OnDemand;
    tmp_OSC8M.Get().bit.RUNSTDBY = CONFIG::RunStandby;

    *reg_OSC8M = tmp_OSC8M;

    return 0;
}

template<typename CONFIG>
error_t OSC8M<CONFIG>::Stop()
{
    RegisterGuard<SYSCTRL_OSC8M_Type> tmp_OSC8M(*reg_OSC8M);
    tmp_OSC8M.Get().bit.ENABLE = 0;
    *reg_OSC8M = tmp_OSC8M;

    return 0;
}

template<typename CONFIG>
frequency_t OSC8M<CONFIG>::GetFrequency() const
{
    // TODO: get the value from the actual clock register.
    // Using Upper Values
    // 0 - 6MHz
    // 1 - 8MHz
    // 2 - 11MHz
    // 3 - 15MHz

    unsigned uPrescaler = CONFIG_OSC8M_PRESC;
    switch (uPrescaler)
    {
    case 0:
        return 6000000;

    case 1:
        return 8000000;

    case 2:
        return 11000000;

    case 3:
        return 15000000;

    default:
        return 0;
    }
}

template<typename CONFIG>
bool OSC8M<CONFIG>::PollIsRunning() const
{
    return reg_PCLKSR->Get().bit.OSC8MRDY;
}

template<typename CONFIG>
ClockType OSC8M<CONFIG>::GetClockSourceType() const
{
    return ClockType::eOSC8M;
}

} // namespace SAMD
