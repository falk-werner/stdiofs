#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/arg.h"

#include <stddef.h>

static int
rpc_serialize_unknown(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    (void) buffer;
    (void) arg;

    return -1;
}

static int
rpc_deserialize_unknown(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    (void) buffer;
    (void) arg;

    return -1;
}


rpc_serialize_fn *
rpc_get_serializer(int type_id)
{
    switch (type_id)
    {
        case RPC_INT:
            return &rpc_serialize_int;
        case RPC_STRING:
            return &rpc_serialize_string;
        case RPC_SIZE:
            return &rpc_serialize_size;
        case RPC_UINT64:
            return &rpc_serialize_u64;
        default:
            return &rpc_serialize_unknown;
    }
}

rpc_deserialize_fn *
rpc_get_deserializer(int type_id)
{
    switch (type_id)
    {
        case RPC_INT:
            return &rpc_deserialize_int;
        default:
            return &rpc_deserialize_unknown;
    }
}