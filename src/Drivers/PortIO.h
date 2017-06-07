/* 
* PortIO.h
*
* Created: 6/1/2017 8:41:55 PM
* Author: Patrick Rohr
*/


#ifndef __PORTIO_H__
#define __PORTIO_H__

#include <stdint.h>

struct PortIO
{
		volatile uint32_t DIR;
		volatile uint32_t DIRCLR;
		volatile uint32_t DIRSET;
		volatile uint32_t DIRTGL;
		volatile uint32_t OUT;
		volatile uint32_t OUTCLR;
		volatile uint32_t OUTSET;
		volatile uint32_t OUTTGL;
		volatile uint32_t IN;
		volatile uint32_t CTRL;
};

enum _port_io_direction {PORT_IO_INPUT, PORT_IO_OUTPUT};

struct PortIO * port_io_init();
void port_io_set_dir(struct PortIO * port_io_ptr, int pin, enum _port_io_direction dir);
void port_io_toggle(struct PortIO * port_io_ptr, int pin);

#endif //__PORTIO_H__
