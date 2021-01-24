/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include <common/id_traits.hpp>
#include <hal_utils/register.hpp>
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

/*!************************************************************
 * @brief      Enum used to configure which peripheral the clock
 *             should be routed to.
 * @date       January 10, 2019
 * @author     Patrick Rohr
 * @todo       These enums are awkwardly named, rename them.
 **************************************************************/
enum class ClockOutput
{
    eGCLK_DFLL48M_REF,
    eGCLK_DPLL,
    eGCLK_DPLL_32K,
    eGCLK_WDT,
    eGCLK_RTC,
    eGCLK_EIC,
    eGCLK_USB,
    eGCLK_EVSYS_CHANNEL_0,
    eGCLK_EVSYS_CHANNEL_1,
    eGCLK_EVSYS_CHANNEL_2,
    eGCLK_EVSYS_CHANNEL_3,
    eGCLK_EVSYS_CHANNEL_4,
    eGCLK_EVSYS_CHANNEL_5,
    eGCLK_EVSYS_CHANNEL_6,
    eGCLK_EVSYS_CHANNEL_7,
    eGCLK_EVSYS_CHANNEL_8,
    eGCLK_EVSYS_CHANNEL_9,
    eGCLK_EVSYS_CHANNEL_10,
    eGCLK_EVSYS_CHANNEL_11,
    eGCLK_SERCOMx_SLOW,
    eGCLK_SERCOM0_CORE,
    eGCLK_SERCOM1_CORE,
    eGCLK_SERCOM2_CORE,
    eGCLK_SERCOM3_CORE,
    eGCLK_SERCOM4_CORE,
    eGCLK_SERCOM5_CORE,
    eGCLK_TCC0_1,
    eGCLK_TCC2_3,
    eGCLK_TC4_5,
    eGCLK_TC6_7,
    eGCLK_ADC,
    eGCLK_AC_DIG,
    eGCLK_AC_ANA,
    eGCLK_DAC,
    eGCLK_PTC,
    eGCLK_I2S_0,
    eGCLK_I2S_1,

    eCount,
    eInvalid = eCount
};

// TODO: change this to something else probably
using frequency_t = unsigned;

class ClockBase;
using gclk_id_t = id_traits<ClockBase, uint8_t>;

class ClockBase
{
public:
    /**
     * @brief      ClockSource itself is RAII. The Gclk however needs to be
     *             started explicitly to allow for additional runtime
     *             configuration.
     *             TODO: Or does it?
     * @param[in]  id  The gclk identifier
     */
    ClockBase(gclk_id_t id) : m_uGclkId(id)
    {
    }

    virtual ~ClockBase() = default;

    // GCLK Interface
    virtual void Enable(uint32_t uDivisionFactor) = 0;

    virtual void Disable() = 0;

    /**
     * @brief      Adds a peripheral to the clock output.
     * @param[in]  eOutput  The e output
     * @todo       In the future, all peripherals that need a clock source,
     *             need to inherit from an interface that forces them to take a
     *             clock source in their constructor. The peripheral constructor
     * will then automatically add the output to that clock.
     */
    virtual void AddOutput(ClockOutput eOutput) = 0;

    /**
     * @brief      Removes an output.
     * @param[in]  eOutput  The e output
     */
    virtual void RemoveOutput(ClockOutput eOutput) = 0;

    virtual bool IsEnabled() const = 0;

    // Clock Source
    virtual void WaitOnClockIsRunning() const = 0;

    /**
     * @brief      Gets the divided (actual) output frequency.
     *
     * @return     The output frequency.
     */
    virtual frequency_t GetOutputFrequency() const = 0;

protected:
    gclk_id_t m_uGclkId;

public:
    ClockBase(const ClockBase&) = delete;
    ClockBase& operator=(const ClockBase&) = delete;
};

} // namespace SAMD
