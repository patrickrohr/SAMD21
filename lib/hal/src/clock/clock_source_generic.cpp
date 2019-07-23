// Copyright 2019, Patrick Rohr

#include "clock/clock_source_generic.hpp"

namespace SAMD
{

ClockSourceGeneric::ClockSourceGeneric(gclk_id_t id) : m_uGclkId(id), m_bIsStarted(false)
{
    static constexpr gclk_id_t g_uGclkIdMax(9);
    samd_assert(m_uGclkId < g_uGclkIdMax, "GCLK ID out of range: %u", uGclkId);
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
