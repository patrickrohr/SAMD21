/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include <common/error.hpp>
#include <common/id_traits.hpp>
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

class ClockSourceGeneric;
using gclk_id_t = id_traits<ClockSourceGeneric, uint8_t>;

class ClockSourceGeneric
{
public:
    /**
     * @brief      ClockSource itself is RAII. The Gclk however needs to be
     *             started explicitly to allow for additional runtime
     *             configuration.
     *             TODO: Or does it?
     * @param[in]  id  The gclk identifier
     */
    ClockSourceGeneric(gclk_id_t id);

    virtual ~ClockSourceGeneric();

    // GCLK Interface
    error_t Enable(/*output */);

    error_t Disable();

    void SetDivision(uint32_t uDivisionFactor);

    bool IsEnabled() const;

    // Clock Source
    error_t WaitOnClockIsRunning() const;

    // Future?
    // void OnClockReady();
protected:
    error_t WaitOnClockReady();
    virtual frequency_t GetFrequency() const = 0;

private:
    // purely virtuals
    virtual bool PollIsRunning() const           = 0;
    virtual ClockType GetClockSourceType() const = 0;

private:
    gclk_id_t m_uGclkId;
    bool m_bIsStarted;

public:
    ClockSourceGeneric(const ClockSourceGeneric&) = delete;
    ClockSourceGeneric& operator=(const ClockSourceGeneric&) = delete;
};

} // namespace SAMD
