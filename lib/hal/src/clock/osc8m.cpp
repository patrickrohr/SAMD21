// Copyright 2019, Patrick Rohr

#include "clock/osc8m.hpp"
#include "config.h"

namespace SAMD
{

static IoPortRW<Sysctrl> g_ioSysctrl(SYSCTRL);

template<typename CONFIG>
OSC8M<CONFIG>::OSC8M(gclk_id_t id) :
    ClockSourceGeneric(id),
    CONFIG(),
    m_ioSysctrlOsc8m(&g_ioSysctrl->OSC8M),
    m_ioSysctrlPclksr(&g_ioSysctrl->PCLKSR)
{
}

template<typename CONFIG>
OSC8M<CONFIG>::~OSC8M() {
}

template<typename CONFIG>
error_t OSC8M<CONFIG>::StartImpl()
{
    // Leave Factory Values for FRANGE and CALIB

    SYSCTRL_OSC8M_Type reg = m_ioSysctrlOsc8m.Read();

    reg.bit.ENABLE   = 1;
    reg.bit.PRESC    = CONFIG::Prescaler; // prescaler of 1
    reg.bit.ONDEMAND = CONFIG::OnDemand;
    reg.bit.RUNSTDBY = CONFIG::RunStandby;

    m_ioSysctrlOsc8m.Write(reg);

    return 0;
}

template<typename CONFIG>
error_t OSC8M<CONFIG>::StopImpl()
{
    SYSCTRL_OSC8M_Type reg = m_ioSysctrlOsc8m.Read();

    reg.bit.ENABLE = 0;

    m_ioSysctrlOsc8m.Write(reg);

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
bool OSC8M<CONFIG>::PollReady() const
{
    return m_ioSysctrlPclksr->bit.OSC8MRDY;
}

template<typename CONFIG>
ClockType OSC8M<CONFIG>::GetClockSourceType() const
{
    return ClockType::eOSC8M;
}

} // namespace SAMD
