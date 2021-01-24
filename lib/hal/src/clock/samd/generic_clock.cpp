// Copyright 2021, Patrick Rohr

#include "clock/samd/generic_clock.hpp"
#include <cmath>

namespace SAMD
{

GenericClock::GenericClock(gclk_id_t id) : ClockBase(id)
{
}

GenericClock::~GenericClock()
{
    bool isEnabled = IsEnabled();
    samd_assert(
        isEnabled,
        "Clock was never enabled: %u",
        static_cast<unsigned>(uGclkId));

    if (isEnabled)
    {
        Disable();
    }
}

void RegisterSync()
{
    // TODO: Protect register for simulation purposes
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) {}
}

void GenericClock::Enable(uint32_t uDivisionFactor)
{
    GCLK_GENDIV_Type tmp_GENDIV{};
    tmp_GENDIV.bit.ID  = static_cast<uint8_t>(m_uGclkId);
    tmp_GENDIV.bit.DIV = uDivisionFactor;

    GCLK->GENDIV.reg = tmp_GENDIV.reg;
    RegisterSync();

    // Write first byte of GEN CTRL before reading
    // TODO: Could Register_Guard provide support to do this directly?
    *reinterpret_cast<volatile uint8_t*>(&GCLK->GENCTRL) =
        static_cast<uint8_t>(m_uGclkId);
    GCLK_GENCTRL_Type tmp_GENCTRL{ .reg = GCLK->GENCTRL.reg };

    tmp_GENCTRL.bit.GENEN = 1;
    tmp_GENCTRL.bit.SRC   = static_cast<uint32_t>(GetClockSourceType());

    GCLK->GENCTRL.reg = tmp_GENCTRL.reg;
    RegisterSync();
}

void GenericClock::Disable()
{
    *reinterpret_cast<volatile uint8_t*>(&GCLK->GENCTRL) =
        static_cast<uint8_t>(m_uGclkId);
    GCLK_GENCTRL_Type tmp_GENCTRL{ .reg = GCLK->GENCTRL.reg };

    tmp_GENCTRL.bit.GENEN = 0;
    GCLK->GENCTRL.reg     = tmp_GENCTRL.reg;

    RegisterSync();
}

bool GenericClock::IsEnabled() const
{
    // check hardware
    *reinterpret_cast<volatile uint8_t*>(&GCLK->GENCTRL) =
        static_cast<uint8_t>(m_uGclkId);
    GCLK_GENCTRL_Type tmp_GENCTRL{ .reg = GCLK->GENCTRL.reg };

    return tmp_GENCTRL.bit.GENEN;
}

void GenericClock::AddOutput(ClockOutput eOutput)
{
    SetOutput(eOutput, true);
}

void GenericClock::RemoveOutput(ClockOutput eOutput)
{
    SetOutput(eOutput, false);
}

void GenericClock::SetOutput(ClockOutput eOutput, bool enable)
{
    samd_assert(
        static_cast<unsigned>(eOutput) <
            static_cast<unsigned>(ClockOutput::eCount),
        "Invalid ClockOutput %u",
        static_cast<unsigned>(eOutput));

    GCLK_CLKCTRL_Type tmp_CLKCTRL{};

    tmp_CLKCTRL.bit.ID    = m_uGclkId.Get();
    tmp_CLKCTRL.bit.GEN   = static_cast<unsigned>(eOutput);
    tmp_CLKCTRL.bit.CLKEN = static_cast<unsigned>(enable);

    GCLK->CLKCTRL.reg = tmp_CLKCTRL.reg;
    RegisterSync();
}

void GenericClock::WaitOnClockIsRunning() const
{
    while (!PollIsRunning()) {}
}

unsigned GenericClock::GetDivisionFactor() const
{
    *reinterpret_cast<volatile uint8_t*>(&GCLK->GENDIV) =
        static_cast<uint8_t>(m_uGclkId);
    GCLK_GENDIV_Type tmp_GENDIV{ .reg = GCLK->GENDIV.reg };

    *reinterpret_cast<volatile uint8_t*>(&GCLK->GENCTRL) =
        static_cast<uint8_t>(m_uGclkId);
    GCLK_GENCTRL_Type tmp_GENCTRL{ .reg = GCLK->GENCTRL.reg };

    uint32_t uDivisionFactor = tmp_GENDIV.bit.DIV;

    enum class DivisionSelector
    {
        eLinear      = 0,
        eExponential = 1
    };

    DivisionSelector eDivisionSelector =
        static_cast<DivisionSelector>(tmp_GENCTRL.bit.DIVSEL);

    if (DivisionSelector::eExponential == eDivisionSelector)
    {
        uDivisionFactor = std::pow(2, uDivisionFactor + 1);
    }
    else if (uDivisionFactor == 0)
    {
        // Division Factor of 0 is equivalent to 1
        uDivisionFactor = 1;
    }

    samd_assert(uDivisionFactor > 0, "Division Factor cannot be 0!");
    return uDivisionFactor;
}

frequency_t GenericClock::GetOutputFrequency() const
{
    // Division factor never returns 0.
    frequency_t result = GetFrequency() / GetDivisionFactor();
    samd_assert(
        result > 0, "Output frequency must be larger than 0: %u", result);
    return result;
}

} // namespace SAMD
