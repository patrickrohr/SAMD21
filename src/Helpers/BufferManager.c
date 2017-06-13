/*
* BufferManager.c
*
* Created: 6/12/2017 8:31:03 PM
*  Author: Patrick Rohr
*/ 


#include "BufferManager.h"
#include "BufferPool.h"


// PRIVATE
const int _BUFFER_MANAGER_BUFFER_SIZE = 100;
const int _BUFFER_MANAGER_BUFFER_COUNT = 5;

struct _BufferManager {
	struct BufferPool * buffer_pool_ptr;
};

struct _BufferManager * _buffer_manager_get_instance()
{
	
	static struct _BufferManager buffer_manager_obj = 
	{
		.buffer_pool_ptr = buffer_pool_create(_BUFFER_MANAGER_BUFFER_SIZE, _BUFFER_MANAGER_BUFFER_COUNT)
	};
	return &buffer_manager_obj;
}

// PUBLIC
// returns buffer packet data address that can be casted into whatever data type it is
struct Packet * buffer_manager_get_packet()
{
	return buffer_pool_get_packet(_buffer_manager_get_instance()->buffer_pool_ptr);
}

void buffer_manager_free_packet(struct Packet * packet_ptr)
{
	buffer_pool_free_packet(_buffer_manager_get_instance()->buffer_pool_ptr, packet_ptr);
}