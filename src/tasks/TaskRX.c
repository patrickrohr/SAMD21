/*
* TaskRX.c
*
* Created: 7/22/2017 4:26:52 PM
*  Author: Patrick Rohr
*/


#include "TaskRX.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
//#include "timers.h"
#include "PacketManager.h"
#include "UART.h"
#include "PacketType.h"
//#include <string.h>




void task_rx_start()
{
	xTaskCreate(task_rx_run, "rx", 100, (void*) 1, tskIDLE_PRIORITY, NULL); // task function, name, stack_size, xyz, priority, reference
}

void task_rx_run(void * pvParameters)
{
	// Create Q for incoming packets
	QueueHandle_t in_q;
	in_q = xQueueCreate(10, sizeof(Packet_t *));

	UART_t * uart_ptr = uart_get_instance(UART0);
	uart_set_q_rx(uart_ptr, in_q);
	SemaphoreHandle_t sema = uart_get_semaphore_rx(uart_ptr);

	Packet_t * packet_ptr = packet_manager_get_packet();
	uart_set_packet_rx(uart_ptr, packet_ptr);


	Packet_t * packet_received_ptr;

	for(;;)
	{
		while(!xSemaphoreTake(sema, (TickType_t) 1000)); // wait for semaphore to become available

		/* do some fancy packet processing magic? */
		while(xQueueReceive(in_q, &packet_received_ptr, (TickType_t) 10)) // how many ticks to wait?
		{
			// At this point I'm wondering why I even use Queues, since I already have a pointer to the packet inside of here...
			uart_set_packet_rx(uart_ptr, packet_manager_get_packet()); // set new packet.
			
			switch(packet_received_ptr->data_type)
			{
				case PACKET_TYPE_TEXT: ;
					// create a helper function for this TODO
					//TextPacket_t * text_packet_ptr = (TextPacket_t *)&packet_received_ptr->data;
					uart_send_packet(uart_ptr, packet_received_ptr);
					break;
			}
			packet_manager_free_packet(packet_received_ptr);
		}
	}
}