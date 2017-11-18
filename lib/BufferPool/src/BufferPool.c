/*!************************************************************
 * Author: Patrick Rohr
**************************************************************/

#include "BufferPool.h"

typedef struct
{
    Buffer_t* first_free_buffer_ptr;
    uint32_t free_buffer_count; // number of free buffers
    Buffer_t buffers[BUFFER_COUNT];
} BufferPool_t;

/*!************************************************************
 * There can only be one BufferPool per project, so this might
 * as well be static.
**************************************************************/
static BufferPool_t gBufferPool;

void buffer_pool_init()
{
    // init buffer pool
    gBufferPool.free_buffer_count = BUFFER_COUNT;

    Buffer_t* prev_buffer_ptr = 0;
    // now create linked list of buffers
    for (int i = 0; i < BUFFER_COUNT; ++i)
    {
        Buffer_t* buffer_ptr = &gBufferPool.buffers[i];
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
    Buffer_t* buffer_ptr = gBufferPool.first_free_buffer_ptr;
    gBufferPool.first_free_buffer_ptr = buffer_ptr->next_ptr;
    buffer_ptr->next_ptr = 0;
    return buffer_ptr;
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
    buffer_ptr->next_ptr = gBufferPool.first_free_buffer_ptr;
    gBufferPool.first_free_buffer_ptr = buffer_ptr;
}

/*!************************************************************
 * @brief      Checks if the buffer protection has been overwritten.
 * @param      buffer_ptr  The buffer pointer
 * @date       November 17, 2017
 * @author     Patrick Rohr
**************************************************************/
int buffer_pool_check_overflow_protection(Buffer_t * buffer_ptr)
{
    return (buffer_ptr->overflow_protection == (uint32_t)&buffer_ptr->overflow_protection);
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