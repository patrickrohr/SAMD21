/*
 * SAMD21.c
 *
 * Created: 6/6/2017 8:36:51 PM
 * Author : Patrick Rohr
 */ 


//#include "sam.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "TaskRX.h"
#include "TaskTX.h"
#include "UART.h"
#include "IRQ.h"

int main(void)
{
	/* Initialize the SAM system */
	SystemInit();

	/* Initialize UART */
	UART_t * uart_ptr = uart_get_instance(UART0);

	SemaphoreHandle_t sema_rx = xSemaphoreCreateBinary();
	
	// semaphore handle is a void *, therefore i will just go ahead and copy it
	uart_init(uart_ptr, 11, 10, sema_rx);
	
	task_tx_start();
	task_rx_start();
	vTaskStartScheduler(); // this should never return

	return 0;	
}




/*
TODO:
Hardware config.h
	- UART setting
	- baud rate
	- clean config file for hardware so its portable

main loop is doing all the setup of the system, including hardware
	- setup uart 
	- setup dma

	typically all systems set up hardware first

*/