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

#include "BufferManager.h"

#include "UART.h"
#include "PortMapping.h"

//#include "BufferPool.h"


struct BufferTest {
	unsigned int delay;
};


void task_blink_start()
{
	xTaskCreate(task_blink_run, "blink", 100, (void*) 1, tskIDLE_PRIORITY, NULL); // task function, name, stack_size, xyz, priority, reference
}

void task_blink_run(void * pvParameters)
{
	/*
	struct BufferPool * buffer_pool_ptr = NULL;
	struct BPTest * test_number;

	buffer_pool_ptr = buffer_pool_create(100, 5);
	struct Buffer * buffer_ptr = buffer_pool_get_buffer(buffer_pool_ptr);
	test_number = (void *) &buffer_ptr->data;
	test_number->test_number = 1000;
	*/
	struct BufferTest * bt = buffer_manager_get_buffer_data();
	bt->delay = 500;

	struct PortIO * port_io_ptr = port_io_init();
	port_io_set_dir(port_io_ptr, 17, PORT_IO_OUTPUT);

	struct SERCOM * uart = uart_create(SERCOM2);


	for(;;)
	{
		uart_send(uart, 'a');
		//struct BufferTest * test = buffer_manager_get_buffer_data();
		//buffer_manager_free_buffer_data(test);

		//struct Buffer * b = buffer_pool_get_buffer(buffer_pool_ptr); // this should cause an error
		//buffer_pool_free_buffer(buffer_pool_ptr, b);
		//test_number->test_number += 500;
		//bt->delay += 500;
		port_io_toggle(port_io_ptr, 17);
		//size_t asdf = xPortGetFreeHeapSize(); //960 last time I checked
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}