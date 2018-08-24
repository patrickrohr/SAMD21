/*!************************************************************
 * Author: Patrick Rohr
**************************************************************/

#include "BufferPool.h"

/*!************************************************************
 * Private Buffer type
 * Publicly visible Buffer_ hides implementation of _Buffer
**************************************************************/
typedef struct _Buffer
{
    struct _Buffer* next_ptr;
    char data[BUFFER_SIZE];
    uint32_t overflow_protection;
} BufferImpl_t;

typedef struct
{
    BufferImpl_t* first_free_buffer_ptr;
    uint32_t free_buffer_count; // number of free buffers
    BufferImpl_t buffers[BUFFER_COUNT];
} BufferPool_t;

/*!************************************************************
 * There can only be one BufferPool per project, so this might
 * as well be static.
**************************************************************/
static BufferPool_t gBufferPool;

// Private functions.
// TODO: to clean this up I should probably move this to a private header file
Buffer_t* to_buffer(BufferImpl_t* buffer_impl_ptr);
BufferImpl_t* to_buffer_impl(Buffer_t* buffer_ptr);

/*!************************************************************
 * @brief      Initializes all Buffers inside of the BufferPool.
 *             
 * @date       November 23, 2017
 * @author     Patrick Rohr
**************************************************************/
void buffer_pool_init()
{
    // init buffer pool
    gBufferPool.free_buffer_count = BUFFER_COUNT;

    BufferImpl_t* prev_buffer_ptr = 0;
    // now create linked list of buffers
    for (int i = 0; i < BUFFER_COUNT; ++i)
    {
        BufferImpl_t* buffer_ptr = &gBufferPool.buffers[i];
        // use overflow protection address as overflow protection value.
        buffer_ptr->overflow_protection = (uint32_t)&buffer_ptr->overflow_protection;
        if (prev_buffer_ptr != 0)
        {
            // hey, this buffer is the previous buffer's next. That's a mouth full.
            prev_buffer_ptr->next_ptr = buffer_ptr;
        }
        prev_buffer_ptr = buffer_ptr;
    }
}

/*!************************************************************
 * @brief      Sets the first free buffer pointer to the second
 *             free buffer and returns the first free buffer.
 * @return     Pointer to free buffer
 * @date       November 17, 2017
 * @author     Patrick Rohr
**************************************************************/
Buffer_t* buffer_pool_get_buffer()
{
    --(gBufferPool.free_buffer_count);
    BufferImpl_t* buffer_ptr = gBufferPool.first_free_buffer_ptr;
    gBufferPool.first_free_buffer_ptr = buffer_ptr->next_ptr;
    buffer_ptr->next_ptr = 0;
    return to_buffer(buffer_ptr);
}

/*!************************************************************
 * @brief      Sets the first free buffer pointer to the buffer
 *             to be freed. No data is erased in the process.
 * @param      buffer_ptr  The buffer pointer
 * @date       November 17, 2017
 * @author     Patrick Rohr
**************************************************************/
void buffer_pool_free_buffer(Buffer_t* buffer_ptr)
{
    ++(gBufferPool.free_buffer_count);
    BufferImpl_t* buffer_impl_ptr = to_buffer_impl(buffer_ptr);
    buffer_impl_ptr->next_ptr = gBufferPool.first_free_buffer_ptr;
    gBufferPool.first_free_buffer_ptr = buffer_impl_ptr;
}

/*!************************************************************
 * @brief      Checks if the buffer protection has been overwritten.
 * @param      buffer_ptr  The buffer pointer
 * @date       November 17, 2017
 * @author     Patrick Rohr
**************************************************************/
int buffer_pool_check_overflow_protection(Buffer_t * buffer_ptr)
{
    BufferImpl_t* buffer_impl_ptr = to_buffer_impl(buffer_ptr);
    return (buffer_impl_ptr->overflow_protection == (uint32_t)&(buffer_impl_ptr->overflow_protection));
}


unsigned int buffer_pool_get_buffer_size()
{
    return BUFFER_SIZE;
}

unsigned int buffer_pool_get_buffer_count()
{
    return BUFFER_COUNT;
}

unsigned int buffer_pool_get_free_count()
{
    return gBufferPool.free_buffer_count;
}

Buffer_t* to_buffer(BufferImpl_t* buffer_impl_ptr)
{
    return (Buffer_t*)(buffer_impl_ptr->data);
}

BufferImpl_t* to_buffer_impl(Buffer_t* buffer_ptr)
{
    return (BufferImpl_t*)(buffer_ptr - sizeof(BufferImpl_t*));
}