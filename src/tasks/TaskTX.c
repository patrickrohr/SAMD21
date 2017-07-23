/*
* TaskTX.c
*
* Created: 7/22/2017 4:26:52 PM
*  Author: Patrick Rohr
*/ 


#include "TaskTX.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "PacketManager.h"
#include "PacketType.h"
#include "UART.h"
#include <string.h>

void task_tx_start()
{
	xTaskCreate(task_tx_run, "tx", 100, (void*) 1, tskIDLE_PRIORITY, NULL); // task function, name, stack_size, xyz, priority, reference
}

void task_tx_run(void * pvParameters)
{
	// 
	UART_t * uart_ptr = uart_get_instance(UART0);

	// create packet and fill it with textpacket
	Packet_t *  packet_ptr = packet_manager_get_packet();
	packet_ptr->data_type = PACKET_TYPE_TEXT;
	TextPacket_t * text_packet = (TextPacket_t *) &(packet_ptr->data);
	
	// text to be sent
	char * string = "Hello World";

	strcpy((char *)&text_packet->text, string);
	text_packet->length = strlen(string);

	for(;;)
	{
		uart_send_packet(uart_ptr, packet_ptr);
			
		//uart_send(uart_ptr, 'a');
		
		// 1s delay
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}