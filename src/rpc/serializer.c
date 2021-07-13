#include "rpc/serializer.h"
#include "rpc/buffer.h"
#include "rpc/arg.h"

#define RPC_INT_SIZE 4

static void
rpc_serialize_int2buffer(
    int value,
    char * data)
{
    for(size_t i = 0; i < RPC_INT_SIZE; i++)
    {
        char actual = value & 0xff;
        value >>= 8;

        data[RPC_INT_SIZE - i - 1] = actual;
    }
}

static void
rpc_serialize_int(
    struct rpc_buffer * buffer,
    int value)
{
    char data[RPC_INT_SIZE];
    rpc_serialize_int2buffer(value, data);
    rpc_buffer_write(buffer, data, RPC_INT_SIZE);
}

static void
rpc_serialize_finalize(
    struct rpc_buffer * buffer)
{
    char data[RPC_INT_SIZE];
    int size = (int) rpc_buffer_size(buffer);
    rpc_serialize_int2buffer(size, data);
    rpc_buffer_write_at(buffer, 0, data, RPC_INT_SIZE);   
}

int
rpc_serialize(
    struct rpc_buffer * buffer,
    int method_id,
    struct rpc_arg const * args)
{
    int result = 0;
    rpc_buffer_reset(buffer);

    rpc_buffer_skip(buffer, RPC_INT_SIZE);
    rpc_serialize_int(buffer, method_id);

    for(size_t i = 0; (0 == result) && (args[i].direction != RPC_END); i++)
    {
        struct rpc_arg const * arg = &(args[i]);
        
        if (arg->direction == RPC_IN)
        {
            switch (arg->type)
            {
                case RPC_INT:
                    {
                        int * value = arg->value;
                        rpc_serialize_int(buffer, *value);
                    }
                    break;
                case RPC_STRING:
                    {
                        char const * value = arg->value;
                        rpc_serialize_string(buffer, value);
                    }
                    break;
                default:
                    result = -1;
                    break;
            }
        }
    }


    rpc_serialize_finalize(buffer);
    return result;
}
