/*
 * SAMD21.c
 *
 * Created: 6/6/2017 8:36:51 PM
 * Author : Patrick Rohr
 */ 


#include "sam.h"
#include "FreeRTOS.h"
#include "task.h"
#include "TaskTX.h"
#include "TaskBlink.h"
#include "UART.h"


/*void SERCOM2_Handler(void)
{
	//for(;;);
	return;
}*/

int main(void)
{
	/* Initialize the SAM system */
	SystemInit();

	/* Initialize UART */
	UART_t * uart_ptr = uart_get_instance(UART0);
	uart_init(uart_ptr, 11, 10);
	
	task_tx_start();
	vTaskStartScheduler(); // this should never return

	return 0;	
}
