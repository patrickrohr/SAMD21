/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "config.h"

#include "clock/clock_source_generic.hpp"

#include <samd21.h>

namespace SAMD
{

struct Xosc32kConfiguration
{
    static constexpr unsigned Startup         = CONFIG_XOSC32K_STARTUP;
    static constexpr unsigned ExternalEnabled = CONFIG_XOSC32K_XTALEN;
    static constexpr unsigned RunStandby      = CONFIG_XOSC32K_RUNSTDBY;
    static constexpr unsigned OnDemand        = CONFIG_XOSC32K_ONDEMAND;
    static constexpr unsigned WriteLock       = CONFIG_XOSC32K_WRTLOCK;
};

template<typename CONFIG = Xosc32kConfiguration>
class XOSC32K
    : public ClockSourceGeneric
    , private CONFIG
{
public:
    XOSC32K(gclk_id_t id);
    ~XOSC32K() override;

private:
    error_t Start();
    error_t Stop();
    frequency_t GetFrequency() const override;
    bool PollIsRunning() const override;
    ClockType GetClockSourceType() const override;
};

} // namespace SAMD
