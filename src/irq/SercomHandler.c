/*
* SercomHandler.c
*
* Created: 7/22/2017 4:41:08 PM
*  Author: Patrick Rohr
*/


#include "IRQ.h"
#include "samd21.h"
#include "UART.h"

void SERCOM0_Handler(void)
{
	

	UART_t * uart_ptr = uart_get_instance(UART0);
	uart_send(uart_ptr, SERCOM0->USART.DATA.reg & SERCOM_USART_DATA_MASK);

	//for(;;);
	//SERCOM0;

	// clear interrupt
	SERCOM0->USART.INTFLAG.reg = SERCOM_USART_INTFLAG_RXC;
}
