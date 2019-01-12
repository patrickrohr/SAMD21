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
    
    _eINVALID
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
void gclk_enable_peripheral_channel(const Gclk_t* self, enum ClockPeripheral eClockPeripheral);
