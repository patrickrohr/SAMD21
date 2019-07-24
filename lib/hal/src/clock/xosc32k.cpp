/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "clock/xosc32k.hpp"
#include "config.h"

namespace SAMD
{

// This definition should probably be in a private header file.
static IoPortRW<Sysctrl> g_ioSysctrl(SYSCTRL);

template<typename CONFIG>
XOSC32K<CONFIG>::XOSC32K(gclk_id_t id) :
    ClockSourceGeneric(id),
    CONFIG(),
    m_ioSysctrlXosc32k(&g_ioSysctrl->XOSC32K),
    m_ioSysctrlPclksr(&g_ioSysctrl->PCLKSR)
{
    Start();
}

template<typename CONFIG>
XOSC32K<CONFIG>::~XOSC32K()
{
    Stop();
}

template<typename CONFIG>
error_t XOSC32K<CONFIG>::Start()
{
    // Leave Factory Values for FRANGE and CALIB
    SYSCTRL_XOSC32K_Type objXosc32kTmp = m_ioSysctrlXosc32k.Read();

    objXosc32kTmp.bit.STARTUP  = CONFIG::Startup;
    objXosc32kTmp.bit.XTALEN   = CONFIG::ExternalEnabled;
    objXosc32kTmp.bit.EN32K    = 1;
    objXosc32kTmp.bit.RUNSTDBY = CONFIG::RunStandby;
    objXosc32kTmp.bit.ONDEMAND = CONFIG::OnDemand;
    objXosc32kTmp.bit.WRTLOCK  = CONFIG::WriteLock;

    m_ioSysctrlXosc32k.Write(objXosc32kTmp);

    // Separate write to Enable bit as per Datasheet
    objXosc32kTmp.bit.ENABLE = 1;
    m_ioSysctrlXosc32k.Write(objXosc32kTmp);

    return 0;
}

template<typename CONFIG>
error_t XOSC32K<CONFIG>::Stop()
{
    // Leave Factory Values for FRANGE and CALIB
    SYSCTRL_XOSC32K_Type objXosc32kTmp = m_ioSysctrlXosc32k.Read();
    objXosc32kTmp.bit.ENABLE           = 0;
    m_ioSysctrlXosc32k.Write(objXosc32kTmp);

    return 0;
}

template<typename CONFIG>
frequency_t XOSC32K<CONFIG>::GetFrequency() const
{
    return 32768;
}

template<typename CONFIG>
bool XOSC32K<CONFIG>::PollReady() const
{
    return m_ioSysctrlPclksr->bit.XOSC32KRDY;
}

template<typename CONFIG>
ClockType XOSC32K<CONFIG>::GetClockSourceType() const
{
    return ClockType::eXOSC32K;
}

} // namespace SAMD
