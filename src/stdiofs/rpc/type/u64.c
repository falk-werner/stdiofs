#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"
#include "stdiofs/rpc/status.h"

#include <inttypes.h>

#define RPC_U64_SIZE 8

void
rpc_serialize_raw_u64(
    struct rpc_buffer * buffer,
    uint64_t value)
{
    char data[RPC_U64_SIZE];
    for(size_t i = 0; i < RPC_U64_SIZE; i++)
    {
        char actual = value & 0xff;
        value >>= 8;

        data[RPC_U64_SIZE - i - 1] = actual;
    }

    rpc_buffer_write(buffer, data, RPC_U64_SIZE);
}

int
rpc_serialize_u64(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    uint64_t value = *((uint64_t*) arg->value);
    rpc_serialize_raw_u64(buffer, value);
    return 0;
}

int
rpc_deserialize_raw_u64(
    struct rpc_buffer * buffer,
    uint64_t * value)
{
    int result = RPC_BAD_DESERIALIZE_FAILED;

    char * data = rpc_buffer_read(buffer, RPC_U64_SIZE);
    if (NULL != data)
    {
        uint64_t val = 0;
        for(size_t i = 0; i < RPC_U64_SIZE; i++)
        {
            val <<= 8;
            val |= ((unsigned int) data[i]) & 0xff;
        }

        *value = val;
        result = 0;
    }

    return result;
}

extern int
rpc_deserialize_u64(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    return rpc_deserialize_raw_u64(buffer, arg->value);
}
