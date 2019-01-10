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
