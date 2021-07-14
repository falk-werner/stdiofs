#include "stdiofs/rpc/deserializer.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"
#include "stdiofs/rpc/types.h"

#define RPC_OFFSET_METHODID 4
#define RPC_OFFSET_DATA     8

int
rpc_get_method_id(
    struct rpc_buffer const * buffer)
{
    struct rpc_buffer read_buffer;
    read_buffer.data = buffer->data;
    read_buffer.capacity = buffer->size;
    read_buffer.size = RPC_OFFSET_METHODID;

    int method_id = 0;
    int result = rpc_deserialize_raw_int(&read_buffer, &method_id);
    if (0 != result)
    {
        method_id = 0;
    }

    return method_id;
}

int
rpc_deserialize(
    struct rpc_buffer const * buffer,
    int direction,
    struct rpc_arg const * args)
{
    int result = 0;

    struct rpc_buffer read_buffer;
    read_buffer.data = buffer->data;
    read_buffer.capacity = buffer->size;
    read_buffer.size = RPC_OFFSET_DATA;

    for(size_t i = 0; (0 == result) && (args[i].direction != RPC_END); i++)
    {
        struct rpc_arg const * arg = &(args[i]);

        if (arg->direction == direction)
        {
            rpc_deserialize_fn * deserialize = rpc_get_deserializer(arg->type);
            result = deserialize(&read_buffer, arg);
        }
    }

    return result;
}
