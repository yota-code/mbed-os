/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#ifndef BUFFER_H__
#define BUFFER_H__

#include <stdint.h>
#include "sdk_errors.h"
#include "pm_mutex.h"


/**
 * @defgroup pm_buffer Buffer
 * @ingroup peer_manager
 * @{
 * @brief An internal module of @ref peer_manager. This module provides a simple buffer.
 */


#define BUFFER_INVALID_ID 0xFF

#define PM_BUFFER_INIT(p_buffer, n_blocks, block_size, err_code)    \
do                                                                  \
{                                                                   \
    static uint8_t buffer_memory[(n_blocks) * (block_size)];        \
    static uint8_t mutex_memory[MUTEX_STORAGE_SIZE(n_blocks)];      \
    err_code = pm_buffer_init((p_buffer),                           \
                               buffer_memory,                       \
                              (n_blocks) * (block_size),            \
                               mutex_memory,                        \
                               MUTEX_STORAGE_SIZE(n_blocks),        \
                              (n_blocks),                           \
                              (block_size));                        \
} while(0)


typedef struct
{
    uint8_t * p_memory;   /**< The storage for all buffer entries. The size of the buffer must be n_blocks*block_size. */
    uint8_t * p_mutex;    /**< A mutex group with one mutex for each buffer entry. */
    uint32_t  n_blocks;   /**< The number of allocatable blocks in the buffer. */
    uint32_t  block_size; /**< The size of each block in the buffer. */
} pm_buffer_t;

/**@brief Function for initializing a buffer instance.
 *
 * @param[out] p_buffer            The buffer instance to initialize.
 * @param[in]  p_buffer_memory     The memory this buffer will use.
 * @param[in]  buffer_memory_size  The size of p_buffer_memory. This must be at least
 *                                 n_blocks*block_size.
 * @param[in]  p_mutex_memory      The memory for the mutexes. This must be at least
 *                                 @ref MUTEX_STORAGE_SIZE(n_blocks).
 * @param[in]  mutex_memory_size   The size of p_mutex_memory.
 * @param[in]  n_blocks            The number of blocks in the buffer.
 * @param[in]  block_size          The size of each block.
 *
 * @retval NRF_SUCCESS              Successfully initialized buffer instance.
 * @retval NRF_ERROR_INVALID_PARAM  A parameter was 0 or NULL or a size was too small.
 */
ret_code_t pm_buffer_init(pm_buffer_t * p_buffer,
                          uint8_t     * p_buffer_memory,
                          uint32_t      buffer_memory_size,
                          uint8_t     * p_mutex_memory,
                          uint32_t      mutex_memory_size,
                          uint32_t      n_blocks,
                          uint32_t      block_size);


/**@brief Function for acquiring a buffer block in a buffer.
 *
 * @param[in]  p_buffer  The buffer instance acquire from.
 * @param[in]  n_blocks  The number of contiguous blocks to acquire.
 *
 * @return The id of the acquired block, if successful.
 * @retval BUFFER_INVALID_ID  If unsuccessful.
 */
uint8_t pm_buffer_block_acquire(pm_buffer_t * p_buffer, uint32_t n_blocks);


/**@brief Function for getting a pointer to a specific buffer block.
 *
 * @param[in]  p_buffer  The buffer instance get from.
 * @param[in]  id        The id of the buffer to get the pointer for.
 *
 * @return A pointer to the buffer for the specified id, if the id is valid.
 * @retval NULL  If the id is invalid.
 */
uint8_t * pm_buffer_ptr_get(pm_buffer_t * p_buffer, uint8_t id);


/**@brief Function for releasing a buffer block.
 *
 * @param[in]  p_buffer  The buffer instance containing the block to release.
 * @param[in]  id        The id of the block to release.
 */
void pm_buffer_release(pm_buffer_t * p_buffer, uint8_t id);


#endif // BUFFER_H__

/**
 * @}
 */
