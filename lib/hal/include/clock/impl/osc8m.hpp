/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "clock/impl/generic_clock.hpp"

#include <samd21.h>

namespace SAMD
{

class OSC8M final : public GenericClock
{
public:
    OSC8M(gclk_id_t id);
    ~OSC8M() override;

    using ClockBase::Enable;

private:
    error_t Start();
    error_t Stop();
    frequency_t GetFrequency() const override;
    bool PollIsRunning() const override;
    ClockType GetClockSourceType() const override;
};

} // namespace SAMD
