#include "stdiofs/rpc/connection.h"
#include "stdiofs/rpc/buffer.h"

#include <gtest/gtest.h>
#include <unistd.h>

#include <csignal>

namespace 
{

void ignore_handler(int signal_number)
{
    (signal_number);
}

}

TEST(rpc_connection, read)
{
    char expected[] = 
    {
        0x00, 0x00, 0x00, 0x08,
        0x01, 0x02, 0x03, 0x04
    };

    int fds[2];
    int rc = pipe(fds);
    ASSERT_EQ(0, rc);

    rpc_connection connection;
    rpc_connection_init(&connection, fds[0], fds[1], 1);

    struct rpc_buffer write_buffer;
    rpc_buffer_init(&write_buffer, 8);
    rpc_buffer_write(&write_buffer, expected, 8);
    rc = rpc_connection_write(&connection, &write_buffer);
    ASSERT_EQ(0, rc);
    rpc_buffer_cleanup(&write_buffer);

    struct rpc_buffer read_buffer;
    rpc_buffer_init(&read_buffer, 8);
    rc = rpc_connection_read(&connection, &read_buffer);
    ASSERT_EQ(0, rc);
    ASSERT_EQ(8, read_buffer.size);
    ASSERT_EQ(std::string(expected, 8), std::string(read_buffer.data, read_buffer.size));
    rpc_buffer_cleanup(&read_buffer);

    rpc_connection_cleanup(&connection);
}

TEST(rpc_connection, failed_write_closed_fd)
{
    char expected[] = 
    {
        0x00, 0x00, 0x00, 0x08,
        0x01, 0x02, 0x03, 0x04
    };

    int fds[2];
    int rc = pipe(fds);
    ASSERT_EQ(0, rc);
    close(fds[1]);

    rpc_connection connection;
    rpc_connection_init(&connection, fds[0], fds[1], 1);

    struct rpc_buffer write_buffer;
    rpc_buffer_init(&write_buffer, 8);
    rpc_buffer_write(&write_buffer, expected, 8);
    rc = rpc_connection_write(&connection, &write_buffer);
    ASSERT_NE(0, rc);
    rpc_buffer_cleanup(&write_buffer);

    rpc_connection_cleanup(&connection);
}

TEST(rpc_connection, failed_write_closed_peer)
{
    char expected[] = 
    {
        0x00, 0x00, 0x00, 0x08,
        0x01, 0x02, 0x03, 0x04
    };

    struct sigaction new_handler;
    memset(&new_handler, 0, sizeof(struct sigaction));
    new_handler.sa_handler = &ignore_handler;
    struct sigaction old_handler;
    int rc = sigaction(SIGPIPE, &new_handler, &old_handler);
    ASSERT_EQ(0, rc);

    int fds[2];
    rc = pipe(fds);
    ASSERT_EQ(0, rc);
    close(fds[0]);

    rpc_connection connection;
    rpc_connection_init(&connection, fds[0], fds[1], 1);

    struct rpc_buffer write_buffer;
    rpc_buffer_init(&write_buffer, 8);
    rpc_buffer_write(&write_buffer, expected, 8);
    rc = rpc_connection_write(&connection, &write_buffer);
    ASSERT_NE(0, rc);
    rpc_buffer_cleanup(&write_buffer);

    rpc_connection_cleanup(&connection);

    rc = sigaction(SIGPIPE, &old_handler, nullptr);
    ASSERT_EQ(0, rc);
}


TEST(rpc_connection, failed_read_closed_fd)
{
    char expected[] = 
    {
        0x00, 0x00, 0x00, 0x08,
        0x01, 0x02, 0x03, 0x04
    };

    int fds[2];
    int rc = pipe(fds);
    ASSERT_EQ(0, rc);

    rpc_connection connection;
    rpc_connection_init(&connection, fds[0], fds[1], 1);

    struct rpc_buffer write_buffer;
    rpc_buffer_init(&write_buffer, 8);
    rpc_buffer_write(&write_buffer, expected, 8);
    rc = rpc_connection_write(&connection, &write_buffer);
    ASSERT_EQ(0, rc);
    rpc_buffer_cleanup(&write_buffer);

    close(fds[0]);
    struct rpc_buffer read_buffer;
    rpc_buffer_init(&read_buffer, 8);
    rc = rpc_connection_read(&connection, &read_buffer);
    ASSERT_NE(0, rc);
    rpc_buffer_cleanup(&read_buffer);

    rpc_connection_cleanup(&connection);
}

TEST(rpc_connection, failed_read_closed_peer)
{
    char expected[] = 
    {
        0x00, 0x00, 0x00, 0x08,
        0x01, 0x02, 0x03, 0x04
    };

    int fds[2];
    int rc = pipe(fds);
    ASSERT_EQ(0, rc);

    rpc_connection connection;
    rpc_connection_init(&connection, fds[0], fds[1], 1);

    struct rpc_buffer write_buffer;
    rpc_buffer_init(&write_buffer, 8);
    rpc_buffer_write(&write_buffer, expected, 3);
    rc = rpc_connection_write(&connection, &write_buffer);
    ASSERT_EQ(0, rc);
    rpc_buffer_cleanup(&write_buffer);

    close(fds[1]);
    struct rpc_buffer read_buffer;
    rpc_buffer_init(&read_buffer, 8);
    rc = rpc_connection_read(&connection, &read_buffer);
    ASSERT_NE(0, rc);
    rpc_buffer_cleanup(&read_buffer);

    rpc_connection_cleanup(&connection);
}