#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"

#include <inttypes.h>

#define RPC_U64_SIZE 8

int
rpc_serialize_u64(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    uint64_t value = *((uint64_t*) arg->value);

    char data[RPC_U64_SIZE];
    for(size_t i = 0; i < RPC_U64_SIZE; i++)
    {
        char actual = value & 0xff;
        value >>= 8;

        data[RPC_U64_SIZE - i - 1] = actual;
    }

    rpc_buffer_write(buffer, data, RPC_U64_SIZE);
    return 0;
}
