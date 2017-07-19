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


typedef PortGroup PortIO_t; /* In the name of consistency, I declare you PortIO_t! */


enum _port_io_direction {PORT_IO_INPUT, PORT_IO_OUTPUT};
enum _port_io_ports {PORT_A, PORT_B};


PortIO_t * port_io_create(enum _port_io_ports port_group);
void port_io_set_dir(PortIO_t * self, int pin, enum _port_io_direction dir);
void port_io_toggle(PortIO_t * self, int pin);
void port_io_uart_init(PortIO_t * self, int pin_rx, int pin_tx);

#endif //__PORTIO_H__
