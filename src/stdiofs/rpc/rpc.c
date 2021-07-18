#include "stdiofs/rpc/rpc.h"
#include "stdiofs/rpc/connection.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/serializer.h"
#include "stdiofs/rpc/deserializer.h"
#include "stdiofs/rpc/arg.h"
#include "stdiofs/rpc/status.h"

#include <stdlib.h>

struct rpc
{
    struct rpc_connection * connection;
    struct rpc_buffer buffer;
};

struct rpc *
rpc_create(
    struct rpc_connection * connection)
{
    struct rpc * rpc = malloc(sizeof(struct rpc));
    rpc->connection = connection;
    rpc_buffer_init(&rpc->buffer, 0);

    return rpc;
}

void
rpc_release(
    struct rpc * rpc)
{
    rpc_buffer_cleanup(&rpc->buffer);
    rpc_connection_cleanup(rpc->connection);
    free(rpc);
}


int
rpc_invoke(
    struct rpc * rpc,
    int method_id,
    struct rpc_arg const * args)
{
    int result;

    result = rpc_serialize(&rpc->buffer, RPC_IN, method_id, args);
    if (0 != result)
    {
        return result;
    }

    result = rpc_connection_write(rpc->connection, &rpc->buffer);
    if (0 != result)
    {
        return result;
    }

    result = rpc_connection_read(rpc->connection, &rpc->buffer);
    if (0 != result)
    {
        return result;
    }

    int const response_method_id = rpc_get_method_id(&rpc->buffer);
    if (method_id != response_method_id)
    {
        return RPC_BAD_INVALID_RESPONSE_METHOD_ID;
    }

    result = rpc_deserialize(&rpc->buffer, RPC_OUT, args);

    return result;
}
