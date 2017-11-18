/*!************************************************************
 * BufferPool
 * Author: Patrick Rohr
 * Ultra-Lightweight Buffer Pool library implementation.
 * This library is perfect for small embedded systems and protects
 * you from memory leaks and memory fragmentation.
**************************************************************/

#pragma once

#include <stdint.h>
#include <stdbool.h>

/*!************************************************************
 * Whoever is using this must define the values to these two variables.
 * Note: Make sure both values gBufferSize and gBufferCount are divisible
 * by 4 for alignment reasons.
**************************************************************/
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 100
#endif

#ifndef BUFFER_COUNT
#define BUFFER_COUNT 10
#endif


typedef struct Buffer
{
    struct Buffer* next_ptr;
   char data[BUFFER_SIZE];
    uint32_t overflow_protection;
} Buffer_t;

/*!************************************************************
 * @brief      Initializes buffer pool. Must always be called first.
 * @date       November 17, 2017
 * @author     Patrick Rohr
**************************************************************/
void buffer_pool_init();

/*!************************************************************
 * @brief      Returns free buffer
 * @return     Pointer to free buffer
 * @date       November 17, 2017
 * @author     Patrick Rohr
**************************************************************/
Buffer_t* buffer_pool_get_buffer();

/*!************************************************************
 * @brief      Frees allocated buffer to be reused later on.
 *             Note: Data is not erased explicitly.
 * @param      buffer_ptr  The buffer pointer
 * @date       November 17, 2017
 * @author     Patrick Rohr
**************************************************************/
void buffer_pool_free_buffer(Buffer_t* buffer_ptr);

/*!************************************************************
 * @brief      Checks if the buffer protection has been overwritten.
 * @param      buffer_ptr  The buffer pointer
 * @date       November 17, 2017
 * @author     Patrick Rohr
**************************************************************/
int buffer_pool_check_overflow_protection(Buffer_t * buffer_ptr);

/*!************************************************************
 * @brief      Returns the size of the buffers
 * @return     Buffer size in bytes
 * @date       November 17, 2017
 * @author     Patrick Rohr
**************************************************************/
unsigned int buffer_pool_get_buffer_size();

/*!************************************************************
 * @brief      Returns the number of buffers
 * @return     Number of buffers
 * @date       November 17, 2017
 * @author     Patrick Rohr
**************************************************************/
unsigned int buffer_pool_get_buffer_count();

/*!************************************************************
 * @brief      Gets the number of buffers the user has left to allocate.
 * @return     Number of free buffers.
 * @date       November 17, 2017
 * @author     Patrick Rohr
**************************************************************/
unsigned int buffer_pool_get_free_count();