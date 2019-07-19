/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "clock.h"
#include <stdbool.h>

/*!************************************************************
 * @brief      Initialize, must be called before using any Clock Source.
 * @warning    Not thread safe.
 * @date       January 13, 2019
 * @author     Patrick Rohr
**************************************************************/
void clock_init(void);

/*!************************************************************
 * @brief      Determines if Clock Source is running
 * @warning    Not thread safe.
 * @param[in]  eSource  The e source
 * @return     True if it is running
 * @date       January 19, 2019
 * @author     Patrick Rohr
**************************************************************/
bool clock_is_running(enum ClockSource eSource);

/*!************************************************************
 * @brief      Returns the clock frequency of the selected Clock Source.
 * @warning    Not thread safe.
 * @param[in]  eSource  The e source
 * @return     clock frequency in Hertz
 * @date       January 19, 2019
 * @author     Patrick Rohr
**************************************************************/
unsigned clock_get_frequency(enum ClockSource eSource);

/*!************************************************************
 * @brief      Start OSC32K
 * @warning    Not thread safe.
 * @date       January 17, 2019
 * @author     Patrick Rohr
**************************************************************/
void clock_osc32k_start(void);

/*!************************************************************
 * @brief      Stop OSC32K
 * @warning    Not thread safe.
 * @date       January 13, 2019
 * @author     Patrick Rohr
**************************************************************/
void clock_osc32k_stop(void);

/*!************************************************************
 * @brief      Start DFLL48M
 * @warning    Not thread safe.
 * @param      uSourceFrequency The frequency of the DFLL reference clock.
 * @date       January 13, 2019
 * @author     Patrick Rohr
**************************************************************/
void clock_dfll48m_start(unsigned uSourceFrequency);

/*!************************************************************
 * @brief      Stop DFLL48M
 * @warning    Not thread safe.
 * @date       January 13, 2019
 * @author     Patrick Rohr
**************************************************************/
void clock_dfll48m_stop(void);

/*!************************************************************
 * @brief      Start XOSC32K
 * @warning    Not thread safe.
 * @date       January 13, 2019
 * @author     Patrick Rohr
**************************************************************/
void clock_xosc32k_start(void);

/*!************************************************************
 * @brief      Stop XOSC32K
 * @warning    Not thread safe.
 * @date       January 13, 2019
 * @author     Patrick Rohr
**************************************************************/
void clock_xosc32k_stop(void);

/*!************************************************************
 * @brief      Start OSC8M
 * @warning    Not thread safe.
 * @date       January 15, 2019
 * @author     Patrick Rohr
**************************************************************/
void clock_osc8m_start(void);

/*!************************************************************
 * @brief      Stops OSC8M
 * @warning    Not thread safe.
 * @date       January 15, 2019
 * @author     Patrick Rohr
**************************************************************/
void clock_osc8m_stop(void);
