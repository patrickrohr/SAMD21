/*
 * SAMD21.c
 *
 * Created: 6/6/2017 8:36:51 PM
 * Author : patri
 */ 


#include "sam.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"


void task_blink(void * pvParameters); // is this bad practice?


int main(void)
{
	/* Initialize the SAM system */
	SystemInit();
	
	xTaskCreate(task_blink, "blink", 100, (void*) 1, tskIDLE_PRIORITY, NULL); // task function, name, stack_size, xyz, priority, reference
	vTaskStartScheduler(); // this should never return

	return 0;	
}
