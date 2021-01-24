// Copyright 2019, Patrick Rohr

#include "clock/samd/osc8m.hpp"
#include "config.h"

namespace SAMD
{
static constexpr unsigned gPrescaler  = CONFIG_OSC8M_PRESC;
static constexpr unsigned gOnDemand   = CONFIG_OSC8M_ONDEMAND;
static constexpr unsigned gRunStandby = CONFIG_OSC8M_RUNSTDBY;

static auto reg_SYSCTRL = MakeRegisterGuard(SYSCTRL);
static auto reg_OSC8M   = MakeRegisterGuard(&reg_SYSCTRL->Get().OSC8M);
static auto reg_PCLKSR  = MakeRegisterGuard(&reg_SYSCTRL->Get().PCLKSR);

OSC8M::OSC8M(gclk_id_t id) : GenericClock(id)
{
    Start();
}

OSC8M::~OSC8M()
{
    Stop();
}

error_t OSC8M::Start()
{
    // Leave Factory Values for FRANGE and CALIB
    RegisterGuard<SYSCTRL_OSC8M_Type> tmp_OSC8M(*reg_OSC8M);

    tmp_OSC8M.Get().bit.ENABLE   = 1;
    tmp_OSC8M.Get().bit.PRESC    = gPrescaler; // prescaler of 1
    tmp_OSC8M.Get().bit.ONDEMAND = gOnDemand;
    tmp_OSC8M.Get().bit.RUNSTDBY = gRunStandby;

    *reg_OSC8M = tmp_OSC8M;

    return 0;
}

error_t OSC8M::Stop()
{
    RegisterGuard<SYSCTRL_OSC8M_Type> tmp_OSC8M(*reg_OSC8M);
    tmp_OSC8M.Get().bit.ENABLE = 0;
    *reg_OSC8M                 = tmp_OSC8M;

    return 0;
}

frequency_t OSC8M::GetFrequency() const
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

bool OSC8M::PollIsRunning() const
{
    return reg_PCLKSR->Get().bit.OSC8MRDY;
}

ClockType OSC8M::GetClockSourceType() const
{
    return ClockType::eOSC8M;
}

} // namespace SAMD
