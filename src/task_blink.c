/*
* task_blink.c
*
* Created: 6/6/2017 9:41:24 PM
*  Author: Patrick Rohr
*/

#include "PortIO.h"

void task_blink(void * pvParameters)
{
	struct PortIO * port_io_ptr = port_io_init();
	port_io_set_dir(port_io_ptr, 17, PORT_IO_OUTPUT);

	while(1)
	{
		port_io_toggle(port_io_ptr, 17);
		unsigned int n = 100000;
		while (n--);
	}
}