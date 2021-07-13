#include "rpc/buffer.h"

#include <stdlib.h>
#include <string.h>

#define RPC_BUFFER_MIN_CAPACITY 16

static size_t rpc_max(size_t a, size_t b)
{
    return (a > b) ? a : b;
}


static void
rpc_buffer_reserve(
    struct rpc_buffer * buffer,
    size_t needed)
{
    if (needed >= buffer->capacity)
    {
        size_t new_capacity = buffer->capacity * 2;
        while (needed < new_capacity)
        {
            new_capacity *= 2;
        }

        buffer->data = realloc(buffer->data, new_capacity);
        buffer->capacity = new_capacity;
    }
}


void
rpc_buffer_init(
    struct rpc_buffer * buffer,
    size_t initial_capacity)
{
    size_t const capacity = rpc_max(initial_capacity, RPC_BUFFER_MIN_CAPACITY);

    buffer->data = malloc(capacity);
    buffer->size = 0;
    buffer->capacity = capacity;
}

void
rpc_buffer_cleanup(
    struct rpc_buffer * buffer)
{
    free(buffer->data);
}


void
rpc_buffer_reset(
    struct rpc_buffer * buffer)
{
    buffer->size = 0;
}

extern size_t
rpc_buffer_size(
    struct rpc_buffer * buffer)
{
    return buffer->size;
}

void
rpc_buffer_write(
    struct rpc_buffer * buffer,
    char * data,
    size_t count)
{
    size_t const needed = (buffer->size + count);
    rpc_buffer_reserve(buffer, needed);
    memcpy(&buffer->data[buffer->size], data, count);
    buffer->size += count;
}

void
rpc_buffer_write_at(
    struct rpc_buffer * buffer,
    size_t offset,
    char * data,
    size_t count)
{
    size_t const needed = (offset + count);
    rpc_buffer_reserve(buffer, needed);
    memcpy(&buffer->data[buffer->size], data, count);
}


void
rpc_buffer_skip(
    struct rpc_buffer * buffer,
    size_t count)
{
    size_t const needed = (buffer->size + count);
    rpc_buffer_reserve(buffer, needed);
    buffer->size += count;
}
