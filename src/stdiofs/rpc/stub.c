#include "stdiofs/rpc/stub.h"
#include "stdiofs/rpc/connection.h"
#include "stdiofs/rpc/buffer.h"

#include <stdlib.h>

struct rpc_stub
{
    rpc_stub_callback_fn * callback;
    void * user_data;
    struct rpc_connection * connection;
    struct rpc_buffer buffer;
};

struct rpc_stub *
rpc_stub_create(
    rpc_stub_callback_fn * callback,
    void * user_data,
    struct rpc_connection * connection)
{
    struct rpc_stub * stub = malloc(sizeof(struct rpc_stub));
    stub->callback = callback;
    stub->user_data = user_data;
    stub->connection = connection;
    rpc_buffer_init(&stub->buffer, 0);

    return stub;
}

void
rpc_stub_release(
    struct rpc_stub * stub)
{
    rpc_buffer_cleanup(&stub->buffer);
    rpc_connection_cleanup(stub->connection);
    free(stub);
}

int
rpc_stub_service(
    struct rpc_stub * stub)
{
    int result = rpc_connection_read(stub->connection, &stub->buffer);
    if (0 != result)
    {
        return result;
    }

    result = stub->callback(stub->user_data, &stub->buffer);
    if (0 != result)
    {
        return result;
    }

    result = rpc_connection_write(stub->connection, &stub->buffer);

    return result;
}
