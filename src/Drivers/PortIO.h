/* 
* PortIO.h
*
* Created: 6/1/2017 8:41:55 PM
* Author: Patrick Rohr
*/


#ifndef __PORTIO_H__
#define __PORTIO_H__

#include <stdint.h>
#include "samd21.h" // TODO: refactor so this is all we are using here, get rid of struct.


enum _port_io_direction {PORT_IO_INPUT, PORT_IO_OUTPUT};
enum _port_io_ports {PORT_A, PORT_B};

PortGroup * port_io_init();
void port_io_set_dir(PortGroup * self, int pin, enum _port_io_direction dir);
void port_io_toggle(PortGroup * self, int pin);

#endif //__PORTIO_H__
