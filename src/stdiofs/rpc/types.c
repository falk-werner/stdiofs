#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/arg.h"
#include "stdiofs/rpc/status.h"

#include <stddef.h>

static int
rpc_serialize_unknown(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    (void) buffer;
    (void) arg;

    return RPC_BAD_UNKNOWN_SERIALIZER;
}

static int
rpc_deserialize_unknown(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    (void) buffer;
    (void) arg;

    return RPC_BAD_UNKNOWN_DESERIALIZER;
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
        case RPC_STAT:
            return &rpc_serialize_stat;
        case RPC_DIRBUFFER:
            return &rpc_serialize_dirbuffer;
        case RPC_OFFSET:
            return &rpc_serialize_offset;
        case RPC_MODE:
            return &rpc_serialize_mode;
        case RPC_DEV:
            return &rpc_serialize_dev;
        case RPC_UID:
            return &rpc_serialize_uid;
        case RPC_GID:
            return &rpc_serialize_gid;
        case RPC_BYTES:
            return &rpc_serialize_bytes;
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
        case RPC_STRING:
            return &rpc_deserialize_string;
        case RPC_SIZE:
            return &rpc_deserialize_size;
        case RPC_UINT64:
            return &rpc_deserialize_u64;
        case RPC_STAT:
            return &rpc_deserialize_stat;
        case RPC_DIRBUFFER:
            return &rpc_deserialize_dirbuffer;
        case RPC_OFFSET:
            return &rpc_deserialize_offset;
        case RPC_MODE:
            return &rpc_deserialize_mode;
        case RPC_DEV:
            return &rpc_deserialize_dev;
        case RPC_UID:
            return &rpc_deserialize_uid;
        case RPC_GID:
            return &rpc_deserialize_gid;
        case RPC_BYTES:
            return &rpc_deserialize_bytes;
        default:
            return &rpc_deserialize_unknown;
    }
}