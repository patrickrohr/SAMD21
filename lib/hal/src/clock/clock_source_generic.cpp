// Copyright 2019, Patrick Rohr

#include "clock/clock_source_generic.hpp"

namespace SAMD
{

static auto reg_GCLK = MakeRegisterGuard(GCLK);
static auto reg_GENCTRL = MakeRegisterGuard(&reg_GCLK->data.GENCTRL);
static auto reg_GENDIV = MakeRegisterGuard(&reg_GCLK->data.GENDIV);

ClockSourceGeneric::ClockSourceGeneric(gclk_id_t id) :
    m_uGclkId(id)
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

error_t RegisterSync()
{
    // TODO: Protect register for simulation purposes
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) {}
    return 0;
}

error_t ClockSourceGeneric::Enable(uint32_t uDivisionFactor)
{
    RegisterGuard<GCLK_GENDIV_Type> regGenDivTemp;
    regGenDivTemp.data.bit.ID = static_cast<uint8_t>(m_uGclkId);
    regGenDivTemp.data.bit.DIV = uDivisionFactor;

    *reg_GENDIV = regGenDivTemp;

    RegisterSync();

    // Write first byte of GEN CTRL before reading
    // TODO: Could Register_Guard provide support to do this directly?
    *reinterpret_cast<volatile uint8_t*>(reg_GENCTRL) = static_cast<uint8_t>(m_uGclkId);
    RegisterGuard<GCLK_GENCTRL_Type> temp_GENCTRL(*reg_GENCTRL);

    temp_GENCTRL.data.bit.GENEN = 1;
    temp_GENCTRL.data.bit.SRC = static_cast<uint32_t>(GetClockSourceType());

    *reg_GENCTRL = temp_GENCTRL;
    RegisterSync();

    return 0;
}

error_t ClockSourceGeneric::Disable()
{

    return 0;
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
