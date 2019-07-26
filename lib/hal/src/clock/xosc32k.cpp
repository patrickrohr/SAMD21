/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "clock/xosc32k.hpp"
#include "config.h"

namespace SAMD
{

// Protected hardware access
static auto reg_SYSCTRL = MakeRegisterGuard(SYSCTRL);
static auto reg_XOSC32K = MakeRegisterGuard(&reg_SYSCTRL->data.XOSC32K);
static auto reg_PCLKSR  = MakeRegisterGuard(&reg_SYSCTRL->data.PCLKSR);

template<typename CONFIG>
XOSC32K<CONFIG>::XOSC32K(gclk_id_t id) : ClockSourceGeneric(id), CONFIG()
{
    Start();
}

template<typename CONFIG>
XOSC32K<CONFIG>::~XOSC32K()
{
    Stop();
}

template<typename CONFIG>
error_t XOSC32K<CONFIG>::Start()
{
    // Leave Factory Values for FRANGE and CALIB
    // TODO: Disable interrupts between reads and writes?
    RegisterGuard<SYSCTRL_XOSC32K_Type> temp_XOSC32K(*reg_XOSC32K);

    tmp_XOSC32K.data.bit.STARTUP  = CONFIG::Startup;
    tmp_XOSC32K.data.bit.XTALEN   = CONFIG::ExternalEnabled;
    tmp_XOSC32K.data.bit.EN32K    = 1;
    tmp_XOSC32K.data.bit.RUNSTDBY = CONFIG::RunStandby;
    tmp_XOSC32K.data.bit.ONDEMAND = CONFIG::OnDemand;
    tmp_XOSC32K.data.bit.WRTLOCK  = CONFIG::WriteLock;

    *reg_XOSC32K = tmp_XOSC32K;

    // Separate write to Enable bit as per Datasheet
    tmp_XOSC32K.data.bit.ENABLE = 1;
    *reg_XOSC32K                = tmp_XOSC32K;

    return 0;
}

template<typename CONFIG>
error_t XOSC32K<CONFIG>::Stop()
{
    // Leave Factory Values for FRANGE and CALIB
    RegisterGuard<SYSCTRL_XOSC32K_Type> temp_XOSC32K(*reg_XOSC32K);
    tmp_XOSC32K.data.bit.ENABLE = 0;
    *reg_XOSC32K                = tmp_XOSC32K;

    return 0;
}

template<typename CONFIG>
frequency_t XOSC32K<CONFIG>::GetFrequency() const
{
    return 32768;
}

template<typename CONFIG>
bool XOSC32K<CONFIG>::PollIsRunning() const
{
    return reg_PCLKSR->data.bit.XOSC32KRDY;
}

template<typename CONFIG>
ClockType XOSC32K<CONFIG>::GetClockSourceType() const
{
    return ClockType::eXOSC32K;
}

} // namespace SAMD
