/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "clock/samd/generic_clock.hpp"

namespace SAMD
{

enum class DfllMode
{
    eOpenLoop,
    eClosedLoop
};

class DFLL48M : public GenericClock
{
public:
    DFLL48M(gclk_id_t id, const ClockBase& sourceClock);
    ~DFLL48M() override;

private:
    error_t Start();
    error_t Stop();
    error_t RegisterSync();
    frequency_t GetFrequency() const override;
    bool PollIsRunning() const override;
    ClockType GetClockSourceType() const override;

private:
    const ClockBase& m_objSourceClock;
};
} // namespace SAMD
