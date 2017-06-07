/*
 * SAMD21.c
 *
 * Created: 6/6/2017 8:36:51 PM
 * Author : patri
 */ 


#include "sam.h"
#include "FreeRTOS.h"
#include "PortIO.h"


int main(void)
{
    /* Initialize the SAM system */
    SystemInit();

	struct PortIO * port_io_ptr = port_io_init();
	port_io_set_dir(port_io_ptr, 17, PORT_IO_OUTPUT);

    /* Replace with your application code */
    while (1) 
    {
		port_io_toggle(port_io_ptr, 17);
		unsigned int n = 10000;
		while (n--);	
    }
}
