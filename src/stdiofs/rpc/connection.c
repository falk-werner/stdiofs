#include "stdiofs/rpc/connection.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/status.h"

#include <unistd.h>
#include <errno.h>

#define RPC_CONNECTION_SIZE_SIZE 4
#define RPC_CONNECTION_MAX_BUFFER_SIZE (16 * 1024 * 1024)

static int
rpc_connection_read_exact(
    int read_fd,
    char * data,
    size_t size)
{
    int result = 0;
    size_t remaining = size;
    size_t offset = 0;

    while (remaining > 0)
    {
        ssize_t length = read(read_fd, &data[offset], remaining);
        if (0 < length)
        {
            remaining -= length;
            offset += length;
        }
        else if (0 == length)
        {
            result = RPC_BAD_CONNECTION_CLOSED;
            break;
        }
        else
        {
            int error_code = errno;
            if ((errno != EAGAIN) && (errno != EWOULDBLOCK) && (errno != EINTR))
            {
                result = RPC_BAD_CONNECTION_READ_ERROR;
                break;
            }
        }
    }

    return result;
}

void
rpc_connection_init(
    struct rpc_connection * connection,
    int read_fd,
    int write_fd,
    int close_on_cleanup)
{
    connection->read_fd  = read_fd;
    connection->write_fd = write_fd;
    connection->close_on_cleanup = close_on_cleanup;
    connection->onerror = NULL;
    connection->user_data = NULL;
}

void
rpc_connection_cleanup(
    struct rpc_connection * connection)
{
    if (0 != connection->close_on_cleanup)
    {
        close(connection->read_fd);
        close(connection->write_fd);
    }
}

int
rpc_connection_write(
    struct rpc_connection * connection,
    struct rpc_buffer const * buffer)
{
    int result = 0;
    size_t remaining = buffer->size;
    size_t offset = 0; 

    while (remaining > 0)
    {
        ssize_t length = write(connection->write_fd, &buffer->data[offset], remaining);
        if (length > 0)
        {
            remaining -= length;
            offset += length;
        }
        else if (-1 == length)
        {
            int error_code = errno;
            if ((errno != EAGAIN) && (errno != EWOULDBLOCK) && (errno != EINTR))
            {
                result = RPC_BAD_CONNECTION_WRITE_ERROR;
                break;
            }
        }
    }

    if ((0 != result) && (NULL != connection->onerror))
    {
        connection->onerror(connection->user_data);
    }
    return result;
}


int
rpc_connection_read(
    struct rpc_connection * connection,
    struct rpc_buffer * buffer)
{
    rpc_buffer_reset(buffer);
    rpc_buffer_reserve(buffer, RPC_CONNECTION_SIZE_SIZE);

    int result = rpc_connection_read_exact(connection->read_fd, buffer->data, RPC_CONNECTION_SIZE_SIZE);
    if (0 == result)
    {
        size_t count = 0;
        for(size_t i = 0; i < RPC_CONNECTION_SIZE_SIZE; i++)
        {
            size_t actual = ((size_t) buffer->data[i]) & 0xff;
            count <<= 8;
            count |= actual;
        }

        if (count < RPC_CONNECTION_MAX_BUFFER_SIZE)
        {
            rpc_buffer_reserve(buffer, count);
            result = rpc_connection_read_exact(connection->read_fd, &buffer->data[RPC_CONNECTION_SIZE_SIZE], count - RPC_CONNECTION_SIZE_SIZE);
            if (0 == result)
            {
                buffer->size = count;
            }
        }
        else
        {
            result = RPC_BAD_MAX_MESSAGE_SIZE_EXCEEDED;
        }

    }

    if ((0 != result) && (NULL != connection->onerror))
    {
        connection->onerror(connection->user_data);
    }
    return result;
}

void
rpc_connection_set_onerror(
    struct rpc_connection * connection,
    rpc_connection_onerror_fn * onerror,
    void * user_data)
{
    connection->onerror = onerror;
    connection->user_data = user_data;
}
