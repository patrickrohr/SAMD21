/*
* PortIO.cpp
*
* Created: 6/1/2017 8:41:55 PM
* Author: Patrick Rohr
*/


#include "PortIO.h"

// TODO: take address out of here.
PortGroup * port_io_init(int port_group)
{
	return &PORT->Group[port_group];
}

void port_io_set_dir(PortGroup * self, int pin, enum _port_io_direction dir)
{
	if (dir)
		self->DIRSET.reg = (1 << pin);
	else
		self->DIRCLR.reg = (1<<pin);
}

void port_io_toggle(PortGroup * self, int pin)
{
	self->OUTTGL.reg = (1<<pin);
}