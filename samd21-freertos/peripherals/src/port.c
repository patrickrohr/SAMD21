/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "port.h"
#include "error.h"
#include <samd21.h>


void port_init(Port_t* self, enum PortGroup ePortGroup)
{
    // TODO: PM Unmasking
    self->uPortGroup = ePortGroup;
}

void port_configure_pin(Port_t* self, unsigned uPinNumber, enum PortPinConfiguration ePinConfig)
{
    // See Pin Write Configuratoin to configure pins simultaneously. We should also support a port_configure_pins_mask();
    // And make this a wrapper for it.

    PORT->Group[self->uPortGroup].OUTCLR.reg = (1 << uPinNumber);

    switch (ePinConfig)
    {
    case ePortInputStandard:
        PORT->Group[self->uPortGroup].PINCFG[uPinNumber].reg = PORT_PINCFG_INEN;
        PORT->Group[self->uPortGroup].DIRCLR.reg             = (1 << uPinNumber);
        break;

    case ePortInputPull:
        PORT->Group[self->uPortGroup].PINCFG[uPinNumber].reg = PORT_PINCFG_PULLEN | PORT_PINCFG_INEN;
        PORT->Group[self->uPortGroup].DIRCLR.reg             = (1 << uPinNumber);
        break;

    case ePortTotemPoleDisabledIn:
        PORT->Group[self->uPortGroup].PINCFG[uPinNumber].reg = 0;
        PORT->Group[self->uPortGroup].DIRSET.reg             = (1 << uPinNumber);
        break;

    case ePortTotemPoleEnabledIn:
        PORT->Group[self->uPortGroup].PINCFG[uPinNumber].reg = PORT_PINCFG_INEN;
        PORT->Group[self->uPortGroup].DIRSET.reg             = (1 << uPinNumber);
        break;

    case ePortOutputPull:
        PORT->Group[self->uPortGroup].PINCFG[uPinNumber].reg = PORT_PINCFG_PULLEN;
        PORT->Group[self->uPortGroup].DIRCLR.reg             = (1 << uPinNumber);
        break;

    case ePortResetAnalog:
        PORT->Group[self->uPortGroup].PINCFG[uPinNumber].reg = 0;
        PORT->Group[self->uPortGroup].DIRCLR.reg             = (1 << uPinNumber);
        break;
    }
}

void port_set_pin_output(Port_t* self, unsigned uPinNumber)
{
    PORT->Group[self->uPortGroup].OUTSET.reg = (1 << uPinNumber);
}

void port_clear_pin_output(Port_t* self, unsigned uPinNumber)
{
    PORT->Group[self->uPortGroup].OUTCLR.reg = (1 << uPinNumber);
}

void port_toggle_pin_output(Port_t* self, unsigned uPinNumber)
{
    PORT->Group[self->uPortGroup].OUTTGL.reg = (1 << uPinNumber);
}

int port_read_pin_input(Port_t* self, unsigned uPinNumber)
{
    return PORT->Group[self->uPortGroup].IN.reg & (1 << uPinNumber);
}
