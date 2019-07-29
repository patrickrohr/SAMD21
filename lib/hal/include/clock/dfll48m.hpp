/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "config.h"

#include "clock/clock_source_generic.hpp"
#include <hal_utils/io_port.hpp>

namespace SAMD
{

enum class DfllMode
{
    eOpenLoop,
    eClosedLoop
};

struct Dfll48mConfiguration
{
    // TODO: Clean up DataTypes
    static constexpr unsigned MultiplierCoarseStep = CONFIG_DFLL48M_MUL_CSTEP;
    static constexpr unsigned MultiplierFineStep   = CONFIG_DFLL48M_MUL_FSTEP;
    static constexpr unsigned RunStandby         = CONFIG_DFLL48M_CTRL_RUNSTDBY;
    static constexpr unsigned OnDemand           = CONFIG_DFLL48M_CTRL_ONDEMAND;
    static constexpr unsigned CtrlMode           = CONFIG_DFLL48M_CTRL_MODE;
    static constexpr unsigned CtrlWaitlock       = CONFIG_DFLL48M_CTRL_WAITLOCK;
    static constexpr unsigned CtrlQlDisable      = CONFIG_DFLL48M_CTRL_QLDIS;
    static constexpr unsigned CtrlCcDisable      = CONFIG_DFLL48M_CTRL_CCDIS;
    static constexpr unsigned CtrlBplckc         = CONFIG_DFLL48M_CTRL_BPLCKC;
    static constexpr unsigned CtrlLlaw           = CONFIG_DFLL48M_CTRL_LLAW;
    static constexpr unsigned CtrlStable         = CONFIG_DFLL48M_CTRL_STABLE;
    static constexpr frequency_t TargetFrequency = 48000000;
    static constexpr DfllMode Mode               = DfllMode::eClosedLoop;
};

template<typename CONFIG = Dfll48mConfiguration>
class DFLL48M
    : public ClockSourceGeneric
    , private CONFIG
{
public:
public:
    DFLL48M(gclk_id_t id, const ClockSourceGeneric& sourceClock);
    ~DFLL48M() override;

private:
    error_t Start();
    error_t Stop();
    error_t RegisterSync();
    frequency_t GetFrequency() const override;
    bool PollIsRunning() const override;
    ClockType GetClockSourceType() const override;

private:
    const ClockSourceGeneric& m_objSourceClock;
};
} // namespace SAMD
