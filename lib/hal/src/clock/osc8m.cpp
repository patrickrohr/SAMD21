// Copyright 2019, Patrick Rohr

#include "clock/osc8m.hpp"
#include "config.h"

namespace SAMD
{

static IoPortRW<Sysctrl> g_ioSysctrl(SYSCTRL);

OSC8M::OSC8M() :
OSC8M::OSC8M(gclk_id_t id) :
    ClockSourceGeneric(id),
    m_ioSysctrlOsc8m(&g_ioSysctrl->OSC8M),
    m_ioSysctrlPclksr(&g_ioSysctrl->PCLKSR)
{
}

error_t OSC8M::StartImpl()
{
    // Leave Factory Values for FRANGE and CALIB

    SYSCTRL_OSC8M_Type reg = m_ioSysctrlOsc8m.Read();

    reg.bit.ENABLE   = 1;
    reg.bit.PRESC    = CONFIG_OSC8M_PRESC; // prescaler of 1
    reg.bit.ONDEMAND = CONFIG_OSC8M_ONDEMAND;
    reg.bit.RUNSTDBY = CONFIG_OSC8M_RUNSTDBY;

    m_ioSysctrlOsc8m.Write(reg);

    return 0;
}

error_t OSC8M::StopImpl()
{
    SYSCTRL_OSC8M_Type reg = m_ioSysctrlOsc8m.Read();

    reg.bit.ENABLE = 0;

    m_ioSysctrlOsc8m.Write(reg);

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

bool OSC8M::PollReady() const
{
    return m_ioSysctrlPclksr->bit.OSC8MRDY;
}

ClockType OSC8M::GetClockSourceType() const
{
    return ClockType::eOSC8M;
}

} // namespace SAMD
