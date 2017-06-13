/*
* BufferManager.h
*
* Created: 6/12/2017 8:31:12 PM
*  Author: Patrick Rohr
*/ 

#ifndef __BUFFER_MANAGER_H__
#define __BUFFER_MANAGER_H__


//enum buffer_manager_buffer_size { BUFFER_SMALL, BUFFER_MEDIUM, BUFFER_LARGE }; // abstract this away

//enum buffer_manager_data_type { BUFFER_UART_PACKET };


/*
TODO:
Having a BufferManager will eventually give me the possibility to abstract away different
buffer sizes and automatically choosing the most efficient buffer size for a certain data packet.
At the moment, it pretty much serves as a 1-to-1 translation layer to BufferPool, but this WILL change.
*/
void * buffer_manager_get_buffer_data();
void buffer_manager_free_buffer_data(void * packet_data_ptr);

#endif