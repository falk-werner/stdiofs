#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"
#include "stdiofs/rpc/status.h"

#include <stddef.h>

#define RPC_SIZE_SIZE 4

extern void
rpc_serialize_raw_u32(
    struct rpc_buffer * buffer,
    uint32_t value)
{
    char data[RPC_SIZE_SIZE];
    for(size_t i = 0; i < RPC_SIZE_SIZE; i++)
    {
        char actual = value & 0xff;
        value >>= 8;

        data[RPC_SIZE_SIZE - i - 1] = actual;
    }

    rpc_buffer_write(buffer, data, RPC_SIZE_SIZE);
}

int
rpc_serialize_size(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    size_t * value = arg->value;
    rpc_serialize_raw_u32(buffer, (uint32_t) *value);
    return 0;
}

int
rpc_deserialize_raw_u32(
    struct rpc_buffer * buffer,
    uint32_t * value)
{
    int result = RPC_BAD_DESERIALIZE_FAILED;

    char * data = rpc_buffer_read(buffer, RPC_SIZE_SIZE);
    if (NULL != data)
    {
        uint32_t val = 0;
        for(size_t i = 0; i < RPC_SIZE_SIZE; i++)
        {
            val <<= 8;
            val |= ((unsigned int) data[i]) & 0xff;
        }

        *value = val;
        result = 0;
    }

    return result;

}

