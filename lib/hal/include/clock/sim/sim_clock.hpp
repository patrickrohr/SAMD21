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
    SimClock(gclk_id_t id) : ClockBase(id), m_isEnabled(false)
    {
    }
    // for DFLL support
    SimClock(gclk_id_t id, const ClockBase& sourceClock) :
        ClockBase(id), m_isEnabled(false)
    {
    }
    ~SimClock() override = default;

    using ClockBase::Enable;
    error_t Enable(uint32_t uDivisionFactor) override
    {
        m_isEnabled = true;
        return 0;
    }

    error_t Disable() override
    {
        m_isEnabled = false;
        return 0;
    }

    void AddOutput(ClockOutput eOutput) override
    {
    }
    void RemoveOutput(ClockOutput eOutput) override
    {
    }

    bool IsEnabled() const override
    {
        return m_isEnabled;
    }
    // consider adding a template parameter for more accurate simulation
    frequency_t GetOutputFrequency() const override
    {
        return 8000000; /* 8MHz */
    }

    error_t WaitOnClockIsRunning() const override
    {
        return 0;
    }

private:
    bool m_isEnabled;
};

} // namespace SAMD
