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
 * @brief      Enum used to configure which peripheral the clock
 *             should be routed to.
 * @date       January 10, 2019
 * @author     Patrick Rohr
**************************************************************/
enum ClockPeripheral
{
    GCLK_DFLL48M_REF,
    GCLK_DPLL,
    GCLK_DPLL_32K,
    GCLK_WDT,
    GCLK_RTC,
    GCLK_EIC,
    GCLK_USB,
    GCLK_EVSYS_CHANNEL_0,
    GCLK_EVSYS_CHANNEL_1,
    GCLK_EVSYS_CHANNEL_2,
    GCLK_EVSYS_CHANNEL_3,
    GCLK_EVSYS_CHANNEL_4,
    GCLK_EVSYS_CHANNEL_5,
    GCLK_EVSYS_CHANNEL_6,
    GCLK_EVSYS_CHANNEL_7,
    GCLK_EVSYS_CHANNEL_8,
    GCLK_EVSYS_CHANNEL_9,
    GCLK_EVSYS_CHANNEL_10,
    GCLK_EVSYS_CHANNEL_11,
    GCLK_SERCOMx_SLOW,
    GCLK_SERCOM0_CORE,
    GCLK_SERCOM1_CORE,
    GCLK_SERCOM2_CORE,
    GCLK_SERCOM3_CORE,
    GCLK_SERCOM4_CORE,
    GCLK_SERCOM5_CORE,
    GCLK_TCC0_1,
    GCLK_TCC2_3,
    GCLK_TC4_5,
    GCLK_TC6_7,
    GCLK_ADC,
    GCLK_AC_DIG,
    GCLK_AC_ANA,
    GCLK_DAC,
    GCLK_PTC,
    GCLK_I2S_0,
    GCLK_I2S_1,
    
    _INVALID
};

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
 * @brief      Initialize Generic Clock Controller
 * @param      self         The handle
 * @param[in]  uId          The Generic Clock Controller ID. 0 is the reference for DFLL48M
 * @param[in]  eGclkSource  The \enum ClockSource
 * @date       January 8, 2019
 * @author     Patrick Rohr
**************************************************************/
void gclk_init(Gclk_t* self, uint8_t uId, enum ClockSource eGclkSource);

/*!************************************************************
 * @brief      Enable Generic Clock Generator
 * @param[in]  self  The handle
 * @date       January 8, 2019
 * @author     Patrick Rohr
**************************************************************/
void gclk_enable_generator(const Gclk_t* self);

/*!************************************************************
 * @brief      Enables Generic Clock Multiplexer
 * @param[in]  self       The object
 * @param[in]  <unnamed>  { parameter_description }
 * @date       January 10, 2019
 * @author     Patrick Rohr
**************************************************************/
void gclk_enable_mux(const Gclk_t* self, enum ClockPeripheral eClockPeripheral);
