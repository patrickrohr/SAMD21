/*
* PortIO.cpp
*
* Created: 6/1/2017 8:41:55 PM
* Author: Patrick Rohr
*/


#include "PortIO.h"

// PRIVATE
/*
* Sets up SERCOM multiplexing
* In the future, once I am using multiple different SERCOMs, this can be set up to take another argument that represents the multiplexing mode
* For right now, Mode C is fix coded for uart use.
*/
void _port_io_sercom_mux(PortIO_t * self, int pin)
{
	self->PINCFG[pin].bit.PMUXEN = 1; // TX port PA10
	self->PMUX[pin / 2].reg |= (pin % 2) ? PORT_PMUX_PMUXO_C : PORT_PMUX_PMUXE_C;
	
}


//PUBLIC
/*
* Creates PortIO and returns handle.
*/
// TODO: change port_group int to enum
PortIO_t * port_io_create(enum _port_io_ports port_group)
{
	return &PORT->Group[port_group];
}

void port_io_set_dir(PortIO_t * self, int pin, enum _port_io_direction dir)
{
	if (dir)
		self->DIRSET.reg = (1 << pin);
	else
		self->DIRCLR.reg = (1<<pin);
}

void port_io_toggle(PortIO_t * self, int pin)
{
	self->OUTTGL.reg = (1<<pin);
}

void port_io_uart_init(PortIO_t * self, int pin_rx, int pin_tx)
{
	// TODO: Figure out which portgroup pins are on.
	// For now, just assume portgroup 0

	// TX pullup output
	self->DIRSET.reg = (1 << pin_tx);
	self->OUTSET.reg = (1 << pin_tx);

	// RX pullup input
	self->PINCFG[pin_rx].bit.INEN	= 1;

	// Port MUX init
	_port_io_sercom_mux(self, pin_tx);
	_port_io_sercom_mux(self, pin_rx);
}