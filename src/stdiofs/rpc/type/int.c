#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"

#define RPC_INT_SIZE 4

void
rpc_serialize_raw_int(
    struct rpc_buffer * buffer,
    int value)
{
    char data[RPC_INT_SIZE];

    for(size_t i = 0; i < RPC_INT_SIZE; i++)
    {
        char actual = value & 0xff;
        value >>= 8;

        data[RPC_INT_SIZE - i - 1] = actual;
    }

    rpc_buffer_write(buffer, data, RPC_INT_SIZE);
}


int
rpc_serialize_int(
    struct rpc_buffer * buffer,
    struct rpc_arg const *arg)
{
    int * value = arg->value;
    rpc_serialize_raw_int(buffer, *value);
    
    return 0;
}
