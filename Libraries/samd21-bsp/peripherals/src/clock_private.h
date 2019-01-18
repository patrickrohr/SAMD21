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
 * @brief      Initialize, must be called before using any clock source.
 * @date       January 13, 2019
 * @author     Patrick Rohr
**************************************************************/
void clock_init(void);


bool clock_is_running(enum ClockSource eSource);

/*!************************************************************
 * @brief      Start OSC32K
 * @param[in]  cbOnReady  The callback on ready
 * @date       January 17, 2019
 * @author     Patrick Rohr
**************************************************************/
void clock_osc32k_start(void);

/*!************************************************************
 * @brief      Stop OSC32K
 * @date       January 13, 2019
 * @author     Patrick Rohr
**************************************************************/
void clock_osc32k_stop(void);

/*!************************************************************
 * @brief      Start DFLL48M
 * @param[in]  cbOnReady  The callback on ready
 * @date       January 13, 2019
 * @author     Patrick Rohr
**************************************************************/
void clock_dfll48m_start(void);

/*!************************************************************
 * @brief      Stop DFLL48M
 * @date       January 13, 2019
 * @author     Patrick Rohr
**************************************************************/
void clock_dfll48m_stop(void);

/*!************************************************************
 * @brief      Start XOSC32K
 * @param[in]  cbOnReady  The callback on ready
 * @date       January 13, 2019
 * @author     Patrick Rohr
**************************************************************/
void clock_xosc32k_start(void);

/*!************************************************************
 * @brief      Stop XOSC32K
 * @date       January 13, 2019
 * @author     Patrick Rohr
**************************************************************/
void clock_xosc32k_stop(void);

/*!************************************************************
 * @brief      Start OSC8M
 * @param[in]  cbOnReady  The callback on ready
 * @date       January 15, 2019
 * @author     Patrick Rohr
**************************************************************/
void clock_osc8m_start(void);

/*!************************************************************
 * @brief      Stops OSC8M
 * @date       January 15, 2019
 * @author     Patrick Rohr
**************************************************************/
void clock_osc8m_stop(void);
