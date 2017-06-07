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
	return *((struct PortIO*)0x41004400);
}

void port_io_set_dir(struct PortIO * port_io_ptr, int pin, enum _port_io_direction dir)
{
	if (dir)
	port_io_ptr->DIRSET = (1<<pin);
	else
	port_io_ptr->DIRCLR = (1<<pin);
}

void port_io_toggle(struct PortIO * port_io_ptr, int pin)
{
	port_io_ptr->OUTTGL = (1<<pin);
}