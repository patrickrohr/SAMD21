/*
* task_blink.c
*
* Created: 6/6/2017 9:41:24 PM
*  Author: Patrick Rohr
*/

#include "TaskBlink.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "PortIO.h"

void task_blink_start()
{
	xTaskCreate(task_blink_run, "blink", 100, (void*) 1, tskIDLE_PRIORITY, NULL); // task function, name, stack_size, xyz, priority, reference
}


void task_blink_run(void * pvParameters)
{
	struct PortIO * port_io_ptr = port_io_init();
	port_io_set_dir(port_io_ptr, 17, PORT_IO_OUTPUT);

	for(;;)
	{
		port_io_toggle(port_io_ptr, 17);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}