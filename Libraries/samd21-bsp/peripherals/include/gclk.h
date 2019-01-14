/**************************************************************
 *                                                            *
 *               Copyright (c) 2018 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "clock.h"
#include <samd21.h>
#include <stdint.h>

/*!************************************************************
 * TODOs:
 *  - Add low-power and sleep settings
 *  - Add functionality to support changing clock source
 *  - Add functionality to support changing division factor
**************************************************************/

/*!************************************************************
 * @brief      Clock Sources available on the SAMD21
 * @date       January 9, 2019
 * @author     Patrick Rohr
**************************************************************/
enum ClockSource
{
    eDFLL48M   = GCLK_SOURCE_DFLL48M,   // DFLL48M output
    eFDPLL     = GCLK_SOURCE_FDPLL,     // FDPLL output
    eGCLKGEN1  = GCLK_SOURCE_GCLKGEN1,  // Generic clock generator 1 output
    eGCLKIN    = GCLK_SOURCE_GCLKIN,    // Generator input pad
    eNUM       = GCLK_SOURCE_NUM,       // Number of Generic Clock Sources
    eOSCULP32K = GCLK_SOURCE_OSCULP32K, // OSCULP32K oscillator output
    eOSC8M     = GCLK_SOURCE_OSC8M,     // OSC8M oscillator output
    eOSC32K    = GCLK_SOURCE_OSC32K,    // OSC32K oscillator outpur
    eXOSC      = GCLK_SOURCE_XOSC,      // XOSC oscillator output
    eXOSC32K   = GCLK_SOURCE_XOSC32K    // XOSC32K oscillator output
};

/*!************************************************************
 * @brief      Enum used to configure which peripheral the clock
 *             should be routed to.
 * @date       January 10, 2019
 * @author     Patrick Rohr
 * @todo       These enums are awkwardly named, rename them.
**************************************************************/
enum ClockOutput
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

    _eClockOutputMax
};

/*!************************************************************
 * @brief      Initialize Generic Clock Controller
 * @date       January 13, 2019
 * @author     Patrick Rohr
**************************************************************/
void gclk_init();

/*!************************************************************
 * @brief      Enable Generic Clock Generator
 * @param[in]  uId  The Generator ID. ID 0 specifies the CPU clock.
 * @date       January 8, 2019
 * @author     Patrick Rohr
**************************************************************/
void gclk_enable_input(uint8_t uId, enum ClockSource eClockSource);

/*!************************************************************
 * @brief      Enables Generic Clock Multiplexer
 * @param[in]  uId               The u identifier
 * @param[in]  eClockOutput  The e clock peripheral
 * @date       January 13, 2019
 * @author     Patrick Rohr
**************************************************************/
void gclk_add_output(uint8_t uId, enum ClockOutput eClockOutput);
