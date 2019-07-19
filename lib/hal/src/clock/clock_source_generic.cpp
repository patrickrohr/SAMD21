// Copyright 2019, Patrick Rohr

#include "clock/clock_source_generic.hpp"

namespace SAMD
{

ClockSourceGeneric::ClockSourceGeneric() : m_bIsStarted(false)
{
}

ClockSourceGeneric::~ClockSourceGeneric()
{
    if (m_bIsStarted)
    {
        Stop();
    }
}

error_t ClockSourceGeneric::Start()
{
    error_t error = StartImpl();
    if (!error)
    {
        m_bIsStarted = true;
    }

    return error;
}

error_t ClockSourceGeneric::Stop()
{
    error_t error = StopImpl();
    if (!error)
    {
        m_bIsStarted = false;
    }

    return error;
}

error_t ClockSourceGeneric::WaitReady() const
{
    while (!PollReady()) {}
    return 0;
}

} // namespace SAMD
