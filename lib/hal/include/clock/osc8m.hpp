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

struct Osc8mConfiguration
{
    static constexpr unsigned Prescaler  = CONFIG_OSC8M_PRESC;
    static constexpr unsigned OnDemand   = CONFIG_OSC8M_ONDEMAND;
    static constexpr unsigned RunStandby = CONFIG_OSC8M_RUNSTDBY;
};

template<typename CONFIG = Osc8mConfiguration>
class OSC8M final : public ClockSourceGeneric, private CONFIG
{
public:
    OSC8M(gclk_id_t id);
    ~OSC8M() override;

private:
    error_t Start();
    error_t Stop();
    frequency_t GetFrequency() const override;
    bool PollIsRunning() const override;
    ClockType GetClockSourceType() const override;
};

} // namespace SAMD
