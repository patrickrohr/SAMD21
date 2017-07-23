/*
* BufferManager.c
*
* Created: 6/12/2017 8:31:03 PM
*  Author: Patrick Rohr
*/ 


#include "PacketManager.h"
#include "BufferPool.h"
#include "GlobalConfig.h"
#include <stdbool.h>


// PRIVATE
//const int _PACKET_MANAGER_BUFFER_SIZE = 100;
//const int _PACKET_MANAGER_BUFFER_COUNT = 5;

struct _PacketManager {
	BufferPool_t * buffer_pool_ptr;
};

struct _PacketManager * _packet_manager_get_instance()
{
	static bool initialized = false;
	static struct _PacketManager packet_manager_obj;
	if(!initialized)
	{
		int buffer_size = MAX_PACKET_DATA_LENGTH + offsetof(Packet_t, data);
		packet_manager_obj.buffer_pool_ptr = buffer_pool_create(buffer_size, BUFFER_COUNT);
		initialized = true;
	}
	return &packet_manager_obj;
}

// PUBLIC
/* This is a Singleton Class.
*  returns buffer packet data address that can be casted into whatever data type it is
*/
Packet_t * packet_manager_get_packet()
{
	return buffer_pool_get_packet(_packet_manager_get_instance()->buffer_pool_ptr);
}

void packet_manager_free_packet(Packet_t * packet_ptr)
{
	//buffer_pool_free_packet(_packet_manager_get_instance()->buffer_pool_ptr, (Packet_t *)(packet_data_ptr - offsetof(Packet_t, data)));
	buffer_pool_free_packet(_packet_manager_get_instance()->buffer_pool_ptr, packet_ptr);
}