/*
* BufferManager.c
*
* Created: 6/12/2017 8:31:03 PM
*  Author: Patrick Rohr
*/ 


#include "BufferManager.h"
#include "BufferPool.h"
#include "stdbool.h"


// PRIVATE
const int _BUFFER_MANAGER_BUFFER_SIZE = 100;
const int _BUFFER_MANAGER_BUFFER_COUNT = 5;

struct _BufferManager {
	struct BufferPool * buffer_pool_ptr;
};

struct _BufferManager * _buffer_manager_get_instance()
{
	static bool initialized = false;
	static struct _BufferManager buffer_manager_obj;
	if(!initialized)
	{
		buffer_manager_obj.buffer_pool_ptr = buffer_pool_create(_BUFFER_MANAGER_BUFFER_SIZE, _BUFFER_MANAGER_BUFFER_COUNT);
		initialized = true;
	}
	return &buffer_manager_obj;
}

// PUBLIC
// returns buffer packet data address that can be casted into whatever data type it is
void * buffer_manager_get_buffer_data()
{
	return &buffer_pool_get_packet(_buffer_manager_get_instance()->buffer_pool_ptr)->data;
}

void buffer_manager_free_buffer_data(void * packet_data_ptr)
{
	
	buffer_pool_free_packet(_buffer_manager_get_instance()->buffer_pool_ptr, (struct Packet *)(packet_data_ptr - offsetof(struct Packet, data)));
}