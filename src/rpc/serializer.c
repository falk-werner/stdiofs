#include "rpc/serializer.h"
#include "rpc/buffer.h"
#include "rpc/arg.h"

#include <inttypes.h>
#include <string.h>

#define RPC_INT_SIZE  4
#define RPC_SIZE_SIZE 4
#define RPC_U64_SIZE 8

static void
rpc_serialize_int(
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

static void
rpc_serialize_size(
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

    rpc_buffer_write(buffer, data, RPC_INT_SIZE);
}

static void
rpc_serialize_u64(
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

static void
rpc_serialize_string(
    struct rpc_buffer * buffer,
    char const * value)
{
    size_t length = strlen(value);

    rpc_serialize_size(buffer, length);
    if (0 < length)
    {
        rpc_buffer_write(buffer, value, length);
    }
}


static void
rpc_serialize_finalize(
    struct rpc_buffer * buffer)
{
    size_t value = rpc_buffer_size(buffer);
    char data[RPC_SIZE_SIZE];
    for(size_t i = 0; i < RPC_SIZE_SIZE; i++)
    {
        char actual = value & 0xff;
        value >>= 8;

        data[RPC_SIZE_SIZE - i - 1] = actual;
    }

    rpc_buffer_write_at(buffer, 0, data, RPC_INT_SIZE);   
}

int
rpc_serialize(
    struct rpc_buffer * buffer,
    int direction,
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
        
        if (arg->direction == direction)
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
                case RPC_SIZE:
                    {
                        size_t * value = arg->value;
                        rpc_serialize_size(buffer, *value);
                    }
                    break;
                case RPC_UINT64:
                    {
                        uint64_t * value = arg->value;
                        rpc_serialize_u64(buffer, *value);
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
