/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "sercom_private.h"
#include "error.h"
#include <samd21.h>
#include <stdbool.h>
#include <stddef.h>


void sercom_init(Sercom_t* self, unsigned uSercomId, enum SercomMode eMode)
{
    self->uSercomId   = uSercomId;
    self->uSercomMode = eMode;

    switch (uSercomId)
    {
    case 0:
        self->pSercom = SERCOM0;
        break;

    case 1:
        self->pSercom = SERCOM1;
        break;

    case 2:
        self->pSercom = SERCOM2;
        break;

    case 3:
        self->pSercom = SERCOM3;
        break;

    case 4:
        self->pSercom = SERCOM4;
        break;

    case 5:
        self->pSercom = SERCOM5;
        break;

    default:
        // Not supported
        assert(0);
        break;
    }

    switch (eMode)
    {
    case eSercomUsartExternalClk:
    case eSercomUsartInternalClk:
        self->pSercom->USART.CTRLA.bit.SWRST = 1;
        while (self->pSercom->USART.SYNCBUSY.bit.SWRST);

        self->pSercom->USART.CTRLA.bit.MODE = eMode;
        break;

    case eSercomSpiSlave:
    case eSercomSpiMaster:
    case eSercomI2cSlave:
    case eSercomI2cMaster:
        // TODO: Implement
        assert(0);
        break;
    }
}
