#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"
#include "stdiofs/rpc/status.h"

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

int
rpc_deserialize_raw_int(
    struct rpc_buffer * buffer,
    int * value)
{
    int result = RPC_BAD_DESERIALIZE_FAILED;

    char * data = rpc_buffer_read(buffer, RPC_INT_SIZE);
    if (NULL != data)
    {
        unsigned int val = 0;
        for(size_t i = 0; i < RPC_INT_SIZE; i++)
        {
            val <<= 8;
            val |= ((unsigned int) data[i]) & 0xff;
        }

        int * p = (int*) &val;
        *value = *p;
        result = 0;
    }

    return result;
}

extern int
rpc_deserialize_int(
    struct rpc_buffer * buffer,
    struct rpc_arg const *arg)
{
    return rpc_deserialize_raw_int(buffer, arg->value);
}

