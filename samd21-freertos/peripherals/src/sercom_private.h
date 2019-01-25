/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "sercom.h"

enum SercomMode
{
    eSercomUsartExternalClk,
    eSercomUsartInternalClk,
    eSercomSpiSlave,
    eSercomSpiMaster,
    eSercomI2cSlave,
    eSercomI2cMaster,
};

void sercom_init(Sercom_t* self, unsigned uSercomId, enum SercomMode eMode);
