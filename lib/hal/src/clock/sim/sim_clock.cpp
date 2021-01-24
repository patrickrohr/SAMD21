// Copyright 2021, Patrick Rohr

#include "clock/sim/sim_clock.hpp"

namespace SAMD
{

SimClock::SimClock(gclk_id_t id) : ClockBase(id), m_isEnabled(false)
{
}

SimClock::SimClock(gclk_id_t id, const ClockBase& sourceClock) :
    ClockBase(id), m_isEnabled(false)
{
}

void SimClock::Enable(uint32_t uDivisionFactor)
{
    m_isEnabled = true;
}

void SimClock::Disable()
{
    m_isEnabled = false;
}

void SimClock::AddOutput(ClockOutput eOutput)
{
}

void SimClock::RemoveOutput(ClockOutput)
{
}

bool SimClock::IsEnabled() const
{
    return m_isEnabled;
}

frequency_t SimClock::GetOutputFrequency() const
{
    // consider adding a template parameter for more accurate simulation
    return 8000000;
}

void SimClock::WaitOnClockIsRunning() const
{
}

} // namespace SAMD
