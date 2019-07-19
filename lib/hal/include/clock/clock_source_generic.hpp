/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include <common/error.hpp>
#include <samd21.h>

namespace SAMD
{

// TODO: consider mapping these values, otherwise these will be a hard to remove
// hw dependency
enum class ClockType
{
    eDFLL48M   = GCLK_SOURCE_DFLL48M,   // DFLL48M output
    eFDPLL     = GCLK_SOURCE_FDPLL,     // FDPLL output
    eGCLKGEN1  = GCLK_SOURCE_GCLKGEN1,  // Generic clock generator 1 output
    eGCLKIN    = GCLK_SOURCE_GCLKIN,    // Generator input pad
    eOSCULP32K = GCLK_SOURCE_OSCULP32K, // OSCULP32K oscillator output
    eOSC8M     = GCLK_SOURCE_OSC8M,     // OSC8M oscillator output
    eOSC32K    = GCLK_SOURCE_OSC32K,    // OSC32K oscillator outpur
    eXOSC      = GCLK_SOURCE_XOSC,      // XOSC oscillator output
    eXOSC32K   = GCLK_SOURCE_XOSC32K,   // XOSC32K oscillator output

    eCount   = GCLK_SOURCE_NUM, // Number of Generic Clock Sources
    eInvalid = 255              // make sure this is actually invalid
};

// TODO: change this to something else probably
using frequency_t = unsigned;

class ClockSourceGeneric
{
public:
    ClockSourceGeneric();
    virtual ~ClockSourceGeneric();

    error_t Start();
    error_t Stop();
    error_t WaitReady() const;

    bool IsRunning() const
    {
        return m_bIsStarted;
    }

    virtual frequency_t GetFrequency() const     = 0;

private:
    // purely virtuals
    virtual error_t StartImpl()                  = 0;
    virtual error_t StopImpl()                   = 0;
    virtual bool PollReady() const               = 0;
    virtual ClockType GetClockSourceType() const = 0;

private:
    bool m_bIsStarted;

public:
    ClockSourceGeneric(const ClockSourceGeneric&) = delete;
    ClockSourceGeneric& operator=(const ClockSourceGeneric&) = delete;
};

} // namespace SAMD
