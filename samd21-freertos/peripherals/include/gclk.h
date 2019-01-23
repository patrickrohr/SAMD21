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
 * @warning    Not thread safe.
 * @date       January 13, 2019
 * @author     Patrick Rohr
**************************************************************/
void gclk_init(void);

/*!************************************************************
 * @brief      Enable Generic Clock Generator
 * @warning    Not thread safe.
 * @param[in]  uGenericClockId  The Generator ID. ID 0 specifies the CPU clock.
 * @date       January 8, 2019
 * @author     Patrick Rohr
**************************************************************/
void gclk_set_input(uint8_t uGenericClockId, enum ClockSource eClockSource);

/*!************************************************************
 * @brief      Sets the Division Factor
 * @warning    Not thread safe.
 * @param[in]  uGenericClockId  The u generic clock identifier
 * @param[in]  uDivisionFactor  The u division factor
 * @date       January 17, 2019
 * @author     Patrick Rohr
**************************************************************/
void gclk_set_division(uint8_t uGenericClockId, uint32_t uDivisionFactor);

/*!************************************************************
 * @brief      Enables the Clock Generator. Call this before adding outputs.
 * @warning    Not thread safe.
 * @param[in]  uGenericClockId  The u generic clock identifier
 * @date       January 17, 2019
 * @author     Patrick Rohr
**************************************************************/
void gclk_enable(uint8_t uGenericClockId);

/*!************************************************************
 * @brief      Sets the output to the Generic Clock.
 * @details    Call this after the gclk has been enabled.
 *             Thread safe.
 * @param[in]  uGenericClockId               The u identifier
 * @param[in]  eClockOutput  The e clock peripheral
 * @date       January 13, 2019
 * @author     Patrick Rohr
**************************************************************/
void gclk_set_output(uint8_t uGenericClockId, enum ClockOutput eClockOutput);

/*!************************************************************
 * @brief      Disables an output in the Generic Clock
 * @details    Thread safe.
 * @param[in]  uGenericClockId  The u generic clock identifier
 * @param[in]  eClockOutput     The e clock output
 * @date       January 17, 2019
 * @author     Patrick Rohr
**************************************************************/
void gclk_disable_output(uint8_t uGenericClockId, enum ClockOutput eClockOutput);
