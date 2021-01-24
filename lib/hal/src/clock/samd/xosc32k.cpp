/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "clock/samd/xosc32k.hpp"
#include "config.h"

namespace SAMD
{
static constexpr unsigned gStartup         = CONFIG_XOSC32K_STARTUP;
static constexpr unsigned gExternalEnabled = CONFIG_XOSC32K_XTALEN;
static constexpr unsigned gRunStandby      = CONFIG_XOSC32K_RUNSTDBY;
static constexpr unsigned gOnDemand        = CONFIG_XOSC32K_ONDEMAND;
static constexpr unsigned gWriteLock       = CONFIG_XOSC32K_WRTLOCK;

XOSC32K::XOSC32K(gclk_id_t id) : GenericClock(id)
{
    Start();
}

XOSC32K::~XOSC32K()
{
    Stop();
}

void XOSC32K::Start()
{
    // Leave Factory Values for FRANGE and CALIB
    // TODO: Disable interrupts between reads and writes?
    SYSCTRL_XOSC32K_Type tmp_XOSC32K{ .reg = SYSCTRL->XOSC32K.reg };

    tmp_XOSC32K.bit.STARTUP  = gStartup;
    tmp_XOSC32K.bit.XTALEN   = gExternalEnabled;
    tmp_XOSC32K.bit.EN32K    = 1;
    tmp_XOSC32K.bit.RUNSTDBY = gRunStandby;
    tmp_XOSC32K.bit.ONDEMAND = gOnDemand;
    tmp_XOSC32K.bit.WRTLOCK  = gWriteLock;

    SYSCTRL->XOSC32K.reg = tmp_XOSC32K.reg;

    // Separate write to Enable bit as per Get()
    tmp_XOSC32K.bit.ENABLE = 1;
    SYSCTRL->XOSC32K.reg   = tmp_XOSC32K.reg;
}

void XOSC32K::Stop()
{
    // Leave Factory Values for FRANGE and CALIB
    SYSCTRL_XOSC32K_Type tmp_XOSC32K{ .reg = SYSCTRL->XOSC32K.reg };
    tmp_XOSC32K.bit.ENABLE = 0;
    SYSCTRL->XOSC32K.reg   = tmp_XOSC32K.reg;
}

frequency_t XOSC32K::GetFrequency() const
{
    return 32768;
}

bool XOSC32K::PollIsRunning() const
{
    return SYSCTRL->PCLKSR.bit.XOSC32KRDY;
}

ClockType XOSC32K::GetClockSourceType() const
{
    return ClockType::eXOSC32K;
}

} // namespace SAMD
