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

XOSC32K::XOSC32K(gclk_id_t id) :
    ClockSourceGeneric(id),
    m_ioSysctrlXosc32k(&g_ioSysctrl->XOSC32K),
    m_ioSysctrlPclksr(&g_ioSysctrl->PCLKSR)
{
}

error_t XOSC32K::StartImpl()
{
    // Leave Factory Values for FRANGE and CALIB
    SYSCTRL_XOSC32K_Type objXosc32kTmp = m_ioSysctrlXosc32k.Read();

    objXosc32kTmp.bit.STARTUP  = CONFIG_XOSC32K_STARTUP;
    objXosc32kTmp.bit.XTALEN   = CONFIG_XOSC32K_XTALEN;
    objXosc32kTmp.bit.EN32K    = 1;
    objXosc32kTmp.bit.RUNSTDBY = CONFIG_XOSC32K_RUNSTDBY;
    objXosc32kTmp.bit.ONDEMAND = CONFIG_XOSC32K_ONDEMAND;
    objXosc32kTmp.bit.WRTLOCK  = CONFIG_XOSC32K_WRTLOCK;

    m_ioSysctrlXosc32k.Write(objXosc32kTmp);

    // Separate write to Enable bit as per Datasheet
    objXosc32kTmp.bit.ENABLE = 1;
    m_ioSysctrlXosc32k.Write(objXosc32kTmp);

    return 0;
}

error_t XOSC32K::StopImpl()
{
    // Leave Factory Values for FRANGE and CALIB
    SYSCTRL_XOSC32K_Type objXosc32kTmp = m_ioSysctrlXosc32k.Read();
    objXosc32kTmp.bit.ENABLE           = 0;
    m_ioSysctrlXosc32k.Write(objXosc32kTmp);

    return 0;
}

frequency_t XOSC32K::GetFrequency() const
{
    return 32768;
}

bool XOSC32K::PollReady() const
{
    return m_ioSysctrlPclksr->bit.XOSC32KRDY;
}

ClockType XOSC32K::GetClockSourceType() const
{
    return ClockType::eXOSC32K;
}

} // namespace SAMD
