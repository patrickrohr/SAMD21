/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "clock/impl/xosc32k.hpp"
#include "config.h"

namespace SAMD
{
static constexpr unsigned gStartup         = CONFIG_XOSC32K_STARTUP;
static constexpr unsigned gExternalEnabled = CONFIG_XOSC32K_XTALEN;
static constexpr unsigned gRunStandby      = CONFIG_XOSC32K_RUNSTDBY;
static constexpr unsigned gOnDemand        = CONFIG_XOSC32K_ONDEMAND;
static constexpr unsigned gWriteLock       = CONFIG_XOSC32K_WRTLOCK;

// Protected hardware access
static auto reg_SYSCTRL = MakeRegisterGuard(SYSCTRL);
static auto reg_XOSC32K = MakeRegisterGuard(&reg_SYSCTRL->Get().XOSC32K);
static auto reg_PCLKSR  = MakeRegisterGuard(&reg_SYSCTRL->Get().PCLKSR);

XOSC32K::XOSC32K(gclk_id_t id) : GenericClock(id)
{
    Start();
}

XOSC32K::~XOSC32K()
{
    Stop();
}

error_t XOSC32K::Start()
{
    // Leave Factory Values for FRANGE and CALIB
    // TODO: Disable interrupts between reads and writes?
    RegisterGuard<SYSCTRL_XOSC32K_Type> tmp_XOSC32K(*reg_XOSC32K);

    tmp_XOSC32K.Get().bit.STARTUP  = gStartup;
    tmp_XOSC32K.Get().bit.XTALEN   = gExternalEnabled;
    tmp_XOSC32K.Get().bit.EN32K    = 1;
    tmp_XOSC32K.Get().bit.RUNSTDBY = gRunStandby;
    tmp_XOSC32K.Get().bit.ONDEMAND = gOnDemand;
    tmp_XOSC32K.Get().bit.WRTLOCK  = gWriteLock;

    *reg_XOSC32K = tmp_XOSC32K;

    // Separate write to Enable bit as per Get()
    tmp_XOSC32K.Get().bit.ENABLE = 1;
    *reg_XOSC32K                 = tmp_XOSC32K;

    return 0;
}

error_t XOSC32K::Stop()
{
    // Leave Factory Values for FRANGE and CALIB
    RegisterGuard<SYSCTRL_XOSC32K_Type> tmp_XOSC32K(*reg_XOSC32K);
    tmp_XOSC32K.Get().bit.ENABLE = 0;
    *reg_XOSC32K                 = tmp_XOSC32K;

    return 0;
}

frequency_t XOSC32K::GetFrequency() const
{
    return 32768;
}

bool XOSC32K::PollIsRunning() const
{
    return reg_PCLKSR->Get().bit.XOSC32KRDY;
}

ClockType XOSC32K::GetClockSourceType() const
{
    return ClockType::eXOSC32K;
}

} // namespace SAMD
