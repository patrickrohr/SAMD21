/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include <stdint.h>

enum PortDirection
{
    ePortInput,
    ePortOutput
};

enum PortPinConfiguration
{
    ePortInputStandard,
    ePortInputPull,
    ePortTotemPoleDisabledIn, // Totem pole output with disabled input
    ePortTotemPoleEnabledIn,  // Totem pole output with enabled input
    ePortOutputPull,
    ePortResetAnalog          // Analog I/O or reset
};

enum PortGroup
{
    ePortA = 0,
    ePortB = 1
};

typedef struct
{
    uint32_t uPortGroup;
} Port_t;

void port_init(Port_t* self, enum PortGroup ePortGroup);

void port_configure_pin(Port_t* self, unsigned uPinNumber, enum PortPinConfiguration ePinConfig);

void port_set_pin_output(Port_t* self, unsigned uPinNumber);

void port_clear_pin_output(Port_t* self, unsigned uPinNumber);

void port_toggle_pin_output(Port_t* self, unsigned uPinNumber);

int port_read_pin_input(Port_t* self, unsigned uPinNumber);
