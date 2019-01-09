/**************************************************************
 *                                                            *
 *               Copyright (c) 2018 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include <samd21.h>
#include <stdint.h>

/*!************************************************************
 * TODOs:
 *  - Hide implementation of Gclk_t
 *  - Add low-power and sleep settings
 *  - Add functionality to support changing clock source
 *  - Add functionality to support changing division factor
**************************************************************/

/*!************************************************************
 * Generic Clock Controller Handle
**************************************************************/
typedef struct
{
    uint8_t  m_uGclkId;           // Generic Clock Control Generator ID
    uint8_t  m_uGclkSource;       // Clock source
    uint16_t m_uGclkGenDiv;       // Division Factor
    uint32_t m_uGclkGenCtrlFlags; // Generator Control Flags
} Gclk_t;

/*!************************************************************
 * @brief      Clock Sources available on the SAMD21
 * @date       January 8, 2019
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
 * @brief      Initialize Generic Clock Controller
 * @param      self         The handle
 * @param[in]  uId          The Generic Clock Controller ID
 * @param[in]  eGclkSource  The \enum ClockSource
 * @date       January 8, 2019
 * @author     Patrick Rohr
**************************************************************/
void gclk_init(Gclk_t* self, uint8_t uId, enum ClockSource eGclkSource);

/*!************************************************************
 * @brief      Enable Generic Clock Source
 * @param[in]  self  The handle
 * @date       January 8, 2019
 * @author     Patrick Rohr
**************************************************************/
void gclk_enable(const Gclk_t* self);
