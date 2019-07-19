/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "clock/clock_source_generic.hpp"
#include <hal_utils/io_port.hpp>

namespace SAMD
{

class DFLL48M : public ClockSourceGeneric
{
public:
    enum class Mode
    {
        eOpenLoop,
        eClosedLoop
    };

public:
    DFLL48M(const ClockSourceGeneric& sourceClock);
    ~DFLL48M() override = default;

    void SetTargetFrequency(frequency_t targetFrequency);
    void SetMode(Mode eMode);

private:
    error_t StartImpl() override;
    error_t StopImpl() override;
    frequency_t GetFrequency() const override;
    bool PollReady() const override;
    ClockType GetClockSourceType() const override;

private:
    IoPortRW<SYSCTRL_DFLLMUL_Type> m_ioSysctrlDfllMultiplier;
    IoPortRW<SYSCTRL_DFLLCTRL_Type> m_ioSysctrlDfllControl;
    IoPortRead<SYSCTRL_PCLKSR_Type> m_ioSysctrlPclksr;
    const ClockSourceGeneric& m_objSourceClock;
    frequency_t m_uTargetFrequency;
    Mode m_eMode;
};
} // namespace SAMD
