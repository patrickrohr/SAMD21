
/*
* BufferPool.h
*
* Created: 6/8/2017 6:36:58 PM
*  Author: Patrick Rohr
*/ 

#ifndef __BUFFER_POOL_H__
#define __BUFFER_POOL_H__

#include <stdint.h>
#include <stddef.h> // includes offsetof()

// allocates and recycles memory in the heap to prevent memory fragmentation
// buffers are organized as linked list
// if a new buffer is "allocated", the first buffer in the linked list will be used
// recycled/freed buffers will be put at the back of the linked list
struct BufferPool
{
	struct Buffer * first_free_buffer_ptr; // points to first free buffer.
	uint16_t size; // size of buffers
	uint16_t count; // number of buffers
};

struct Packet
{
	uint32_t data_type; //typecast from enum packet_data_type
	void * data;
};

struct BufferPool * buffer_pool_create(int size, int count); // creates/allocates memory for buffer pool in heap and returns handle
struct Packet * buffer_pool_get_packet(struct BufferPool * self); // allocates buffer inside buffer pool and returns handle to packet
void buffer_pool_free_packet(struct BufferPool * self, struct Packet * packet_ptr);


#endif