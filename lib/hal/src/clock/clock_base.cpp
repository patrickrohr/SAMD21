// Copyright 2019, Patrick Rohr

#include "clock/clock_base.hpp"

namespace SAMD
{

ClockBase::ClockBase(gclk_id_t id) : m_uGclkId(id)
{
    static constexpr gclk_id_t uMaxGclkId(9);
    samd_assert(m_uGclkId < uMaxGclkId, "GCLK ID out of range: %u", uGclkId);
}

error_t ClockBase::Enable()
{
    return Enable(0);
}

} // namespace SAMD
