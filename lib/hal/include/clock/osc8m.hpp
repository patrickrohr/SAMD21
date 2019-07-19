/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "clock/clock_source_generic.hpp"
#include <hal_utils/io_port.hpp>

#include <samd21.h>

namespace SAMD
{

class OSC8M final : public ClockSourceGeneric
{
public:
    OSC8M();
    ~OSC8M() override = default;

private:
    error_t StartImpl() override;
    error_t StopImpl() override;
    frequency_t GetFrequency() const override;
    bool PollReady() const override;
    ClockType GetClockSourceType() const override;

private:
    IoPortRW<SYSCTRL_OSC8M_Type> m_ioSysctrlOsc8m;
    IoPortRead<SYSCTRL_PCLKSR_Type> m_ioSysctrlPclksr;
};

} // namespace SAMD
