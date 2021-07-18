#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"
#include "stdiofs/rpc/status.h"

#include <stdlib.h>
#include <string.h>

#define RPC_DIRBUFFER_INITIAL_CAPACITY 16

void
rpc_dirbuffer_init(
    struct rpc_dirbuffer * buffer)
{
    buffer->entries = malloc(sizeof(char*) * RPC_DIRBUFFER_INITIAL_CAPACITY);
    buffer->capacity = RPC_DIRBUFFER_INITIAL_CAPACITY;
    buffer->size = 0;
}

void
rpc_dirbuffer_cleanup(
    struct rpc_dirbuffer * buffer)
{
    for(size_t i = 0; i < buffer->size; i++)
    {
        free(buffer->entries[i]);
    }
    free(buffer->entries);
}

int
rpc_dirbuffer_add(
    void * buffer,
    char const * name)
{
    struct rpc_dirbuffer * dirbuffer = buffer;
    if (dirbuffer->size >= dirbuffer->capacity)
    {
        size_t new_capacity = dirbuffer->capacity * 2;
        dirbuffer->entries = realloc(dirbuffer->entries, sizeof(char *) * new_capacity);
        dirbuffer->capacity = new_capacity;
    }

    dirbuffer->entries[dirbuffer->size] = strdup(name);
    dirbuffer->size++;

    return 0;
}

int
rpc_serialize_dirbuffer(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    struct rpc_dirbuffer * dirbuffer = arg->value;

    rpc_serialize_raw_u32(buffer, (uint32_t) dirbuffer->size);
    for(size_t i = 0; i < dirbuffer->size; i++)
    {
        rpc_serialize_raw_string(buffer, dirbuffer->entries[i]);
    }

    return 0;
}

int
rpc_deserialize_dirbuffer(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    void * dirbuffer = arg->value;
    rpc_dirbuffer_add_fn * add = arg->extra;

    uint32_t count;
    int result = rpc_deserialize_raw_u32(buffer, &count);
    for(uint32_t i = 0; (0 == result) && (i < count); i++)
    {
        char * value;
        result = rpc_deserialize_raw_string(buffer, &value);
        if (0 == result)
        {
            add(dirbuffer, value);
        }
    }

    return result;
}