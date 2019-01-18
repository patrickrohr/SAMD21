/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include <samd21.h>

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
