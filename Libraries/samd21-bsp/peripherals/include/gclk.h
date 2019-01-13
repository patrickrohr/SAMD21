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
 * @todo       These enums are awkwardly named, rename them.
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
 * @brief      Initialize Generic Clock Controller
 * @date       January 13, 2019
 * @author     Patrick Rohr
**************************************************************/
void gclk_init();

/*!************************************************************
 * @brief      Configure Generic Clock Controller
 * @param[in]  uId          The Generic Clock Controller ID. 0 is CPU Clock.
 * @param[in]  eGclkSource  The \enum ClockSource
 * @date       January 8, 2019
 * @author     Patrick Rohr
**************************************************************/
void gclk_configure(uint8_t uId, enum ClockSource eGclkSource);

/*!************************************************************
 * @brief      Enable Generic Clock Generator
 * @param[in]  uId  The Generetar ID
 * @date       January 8, 2019
 * @author     Patrick Rohr
**************************************************************/
void gclk_enable_generator(uint8_t uId);

/*!************************************************************
 * @brief      Enables Generic Clock Multiplexer
 * @param[in]  uId               The u identifier
 * @param[in]  eClockPeripheral  The e clock peripheral
 * @date       January 13, 2019
 * @author     Patrick Rohr
**************************************************************/
void gclk_enable_peripheral_channel(uint8_t uId, enum ClockPeripheral eClockPeripheral);
