#include "stdiofs/rpc/serializer.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"
#include "stdiofs/rpc/types.h"

#include <inttypes.h>
#include <string.h>

#define RPC_SIZE_SIZE  4

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

    rpc_buffer_write_at(buffer, 0, data, RPC_SIZE_SIZE);   
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

    rpc_buffer_skip(buffer, RPC_SIZE_SIZE);
    rpc_serialize_raw_int(buffer, method_id);

    for(size_t i = 0; (0 == result) && (args[i].direction != RPC_END); i++)
    {
        struct rpc_arg const * arg = &(args[i]);

        if (arg->direction == direction)
        {
            rpc_serialize_fn * serialize = rpc_get_serializer(arg->type);
            result = serialize(buffer, arg);
        }
    }


    rpc_serialize_finalize(buffer);
    return result;
}
