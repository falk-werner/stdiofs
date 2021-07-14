#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"

#include <stddef.h>

#define RPC_SIZE_SIZE 4

extern void
rpc_serialize_raw_size(
    struct rpc_buffer * buffer,
    size_t value)
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
    rpc_serialize_raw_size(buffer, *value);
    return 0;
}
