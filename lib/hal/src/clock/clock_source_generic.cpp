// Copyright 2019, Patrick Rohr

#include "clock/clock_source_generic.hpp"

namespace SAMD
{

ClockSourceGeneric::ClockSourceGeneric(gclk_id_t id) : m_uGclkId(id)
{
    static constexpr gclk_id_t g_uGclkIdMax(9);
    samd_assert(m_uGclkId < g_uGclkIdMax, "GCLK ID out of range: %u", uGclkId);
}

ClockSourceGeneric::~ClockSourceGeneric()
{
    if (IsEnabled())
    {
        Disable();
    }
}

error_t ClockSourceGeneric::Enable()
{

    return 0;
}

error_t ClockSourceGeneric::Disable()
{

    return 0;
}

void ClockSourceGeneric::SetDivision(uint32_t uDivisionFactor)
{
    (void) uDivisionFactor;
}

bool ClockSourceGeneric::IsEnabled() const
{
    // check hardware
    return false;
}

error_t ClockSourceGeneric::WaitOnClockIsRunning() const
{
    while (!PollIsRunning()) {}
    return 0;
}

} // namespace SAMD
