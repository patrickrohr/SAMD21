
/*
* BufferPool.h
*
* Created: 6/8/2017 6:36:58 PM
*  Author: Patrick Rohr
*/ 

#ifndef __BUFFER_POOL_H__
#define __BUFFER_POOL_H__

#include <stdint.h>

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

struct Buffer
{
	struct Buffer * next_ptr;
	//enum buffer_data_type;
	void * data; // this is as a reference for where the data will start, not an actual pointer. Will mostly contain Packet struct.
};

struct Packet
{
	uint32_t data_type; //typecast from enum packet_data_type
	void * data;
};


struct BufferPool * buffer_pool_create(int size, int count); // creates/allocates memory for buffer pool in heap and returns handle
struct Buffer * buffer_pool_get_buffer(struct BufferPool * self); // allocates buffer inside buffer pool and returns handle to buffer
void buffer_pool_free_buffer(struct BufferPool * self, struct Buffer * buffer_ptr);

#endif