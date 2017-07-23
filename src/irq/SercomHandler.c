/*
* SercomHandler.c
*
* Created: 7/22/2017 4:41:08 PM
*  Author: Patrick Rohr
*/


#include "IRQ.h"
#include "samd21.h"
#include "UART.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "PacketType.h"
#include <stdbool.h>

// TODO: make this code independent from uart ports
void SERCOM0_Handler(void)
{
	if (SERCOM0->USART.INTFLAG.bit.ERROR)
	{


	}
	
	/* RX Complete Interrupt */
	if (SERCOM0->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_RXC)
	{

		UART_t * uart_ptr = uart_get_instance(UART0);

		QueueHandle_t q_rx = uart_get_q_rx(uart_ptr); // alternatively I could access this directly from uart handle
		Packet_t * packet_rx_ptr = uart_get_packet_rx(uart_ptr);
	
		bool packet_full = false; // indicates if a packet is ready to be send to the back of the Q
		/* handle incoming data by datatype (set by taskrx) */
		switch(packet_rx_ptr->data_type)
		{
			case(PACKET_TYPE_TEXT): ;
				TextPacket_t * text_packet_ptr = (TextPacket_t *)&packet_rx_ptr->data;
				packet_full = text_packet_push_char(text_packet_ptr, (char)(SERCOM0->USART.DATA.reg & SERCOM_USART_DATA_MASK));
				break;
		}

		BaseType_t pxHigherPriorityTaskWoken = pdFALSE; // if this is ever set to true, don't forget to call portyield

		// TODO: error handling
		if (packet_full)
		{
			xQueueSendToBackFromISR(q_rx, (void **) &packet_rx_ptr, &pxHigherPriorityTaskWoken);
			/* give semaphore */
			SemaphoreHandle_t sema = uart_get_semaphore_rx(uart_ptr);
			xSemaphoreGiveFromISR(sema, &pxHigherPriorityTaskWoken);
		}

		/* clear interrupt */
		SERCOM0->USART.INTFLAG.reg = SERCOM_USART_INTFLAG_RXC;
	}
}
