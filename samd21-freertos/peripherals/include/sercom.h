/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "stdint.h"
#include <samd21.h>

typedef struct
{
    Sercom*  pSercom;
    uint16_t uSercomMode;
    uint16_t uSercomId;
} Sercom_t;
