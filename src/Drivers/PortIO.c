/*
* PortIO.cpp
*
* Created: 6/1/2017 8:41:55 PM
* Author: Patrick Rohr
*/


#include "PortIO.h"

// TODO: take address out of here.
struct PortIO * port_io_init()
{
	return ((struct PortIO*)0x41004400);
}

void port_io_set_dir(struct PortIO * this, int pin, enum _port_io_direction dir)
{
	if (dir)
	this->DIRSET = (1<<pin);
	else
	this->DIRCLR = (1<<pin);
}

void port_io_toggle(struct PortIO * this, int pin)
{
	this->OUTTGL = (1<<pin);
}