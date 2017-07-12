/*
 * SAMD21.c
 *
 * Created: 6/6/2017 8:36:51 PM
 * Author : Patrick Rohr
 */ 


#include "sam.h"
#include "FreeRTOS.h"
#include "task.h"
#include "TaskBlink.h"


/*void SERCOM2_Handler(void)
{
	//for(;;);
	return;
}*/

int main(void)
{
	/* Initialize the SAM system */
	SystemInit();
	
	task_blink_start();
	vTaskStartScheduler(); // this should never return

	return 0;	
}
