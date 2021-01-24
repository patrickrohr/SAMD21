/**************************************************************
 *                                                            *
 *               Copyright (c) 2021 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "clock/clock_base.hpp"

namespace SAMD
{

class SimClock : public ClockBase
{
public:
    SimClock(gclk_id_t id);
    // for DFLL support
    SimClock(gclk_id_t id, const ClockBase& sourceClock);
    ~SimClock() override = default;

    void Enable(uint32_t uDivisionFactor) override;
    void Disable() override;

    void AddOutput(ClockOutput eOutput) override;
    void RemoveOutput(ClockOutput eOutput) override;

    bool IsEnabled() const override;

    // consider adding a template parameter for more accurate simulation
    frequency_t GetOutputFrequency() const override;

    void WaitOnClockIsRunning() const override;

private:
    bool m_isEnabled;
};

} // namespace SAMD
