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
// recycled/freed buffers will be put in front of the linked list

/* Linus would hate me for this. But damn peer pressure */
typedef struct
{
	struct _Buffer * first_free_buffer_ptr; // points to first free buffer.
	uint16_t size; // size of buffers
	uint16_t count; // number of buffers
} BufferPool_t;

struct _Buffer
{
	struct _Buffer * next_ptr;
	//enum buffer_data_type;
	void * data; // this is as a reference for where the data will start, not an actual pointer. Will mostly contain Packet struct.
};

typedef struct
{
  uint32_t data_type;
  void * data;
} Packet_t;

BufferPool_t * buffer_pool_create(int size, int count); // creates/allocates memory for buffer pool in heap and returns handle
Packet_t * buffer_pool_get_packet(BufferPool_t * self); // allocates buffer inside buffer pool and returns handle to packet
void buffer_pool_free_packet(BufferPool_t * self, Packet_t * packet_ptr);

#endif