/*
* BufferPool.c
*
* Created: 6/8/2017 6:36:48 PM
*  Author: Patrick Rohr
*/

#include "BufferPool.h"
#include <stddef.h> // including NULL pointer
#include <string.h> // memset
#include "FreeRTOS.h" // for use of memory allocation functions
#include "Error.h"


// PRIVATE
// Buffer is completely hidden in order to make sure no one (not even Chuck Norris, maybe, but just maybe John Cena) can override next_ptr



// clears data in buffer
void _buffer_pool_clear_buffer(BufferPool_t * self, struct _Buffer * buffer_ptr)
{
  memset(buffer_ptr->data, 0, self->size);
}

// allocates buffer inside buffer pool and returns handle to buffer
struct _Buffer * _buffer_pool_get_buffer(BufferPool_t * self)
{
  if (!(self->first_free_buffer_ptr == NULL))
  {
    struct _Buffer * buffer_ptr = self->first_free_buffer_ptr; // get first free buffer
    self->first_free_buffer_ptr = buffer_ptr->next_ptr; // set first free buffer to next available buffer
    _buffer_pool_clear_buffer(self, buffer_ptr);
    return buffer_ptr;
  }
  
  error_set(ERROR_BUFFER_POOL_MEMORY_LEAK);
  error_set(ERROR_LEVEL_FATAL);
  return NULL;
}

// adds buffer to the front of free buffer list
void _buffer_pool_free_buffer(BufferPool_t * self, struct _Buffer * buffer_ptr)
{
  buffer_ptr->next_ptr = self->first_free_buffer_ptr;
  self->first_free_buffer_ptr = buffer_ptr;
}


// PUBLIC

// creates/allocates memory for buffer pool in heap and returns handle
BufferPool_t * buffer_pool_create(int size, int count)
{
  BufferPool_t * buffer_pool_ptr = pvPortMalloc(sizeof(BufferPool_t));
  
  struct _Buffer * prev_buffer_ptr = NULL; // used to set up linked list. setting this to null so the stupid warning goes away...
  int buffer_size = sizeof(struct _Buffer) + size;
  
  for(int i = 0; i < count; i++)
  {
    // buffer_size is 0x4 bytes larger than the actual size plus struct buffer, since the void data pointer will be overwritten with data.
    // this will leave enough room for four control bytes at the end of the buffer that serves as overwrite protection
    
    struct _Buffer * buffer_ptr = pvPortMalloc(buffer_size);

    if (i == 0) // first element, therefore there is no previous element
    {
      buffer_pool_ptr->first_free_buffer_ptr = buffer_ptr;
    } else
    {
      prev_buffer_ptr->next_ptr = buffer_ptr;
    }
    prev_buffer_ptr = buffer_ptr;
  }
  if (count > 0)
  {
    prev_buffer_ptr->next_ptr = NULL; // last element in list is not pointing anywhere
  }

  return buffer_pool_ptr;
}

Packet_t * buffer_pool_get_packet(BufferPool_t * self)
{
  struct _Buffer * buffer_ptr = _buffer_pool_get_buffer(self);
  return (Packet_t *) &buffer_ptr->data;
}


void buffer_pool_free_packet(BufferPool_t * self, Packet_t * packet_ptr)
{
  _buffer_pool_free_buffer(self, (struct _Buffer *)(packet_ptr - 1));
}