#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"

#include <gtest/gtest.h>

TEST(rpc_int, serialize)
{
    int value = 0x01020304;
    rpc_buffer buffer;
    rpc_buffer_init(&buffer, 0);
    rpc_arg arg = {nullptr, RPC_IN, RPC_INT, &value, nullptr};

    int result = rpc_serialize_int(&buffer, &arg);

    ASSERT_EQ(0, result);
    ASSERT_EQ(4, buffer.size);

    char expected[]= {0x01, 0x02, 0x03, 0x04};
    ASSERT_EQ(std::string(expected, 4), std::string(buffer.data, buffer.size));

    rpc_buffer_cleanup(&buffer);
}

TEST(rpc_int, serialize_raw)
{
    rpc_buffer buffer;
    rpc_buffer_init(&buffer, 0);

    rpc_serialize_raw_int(&buffer, 0);

    ASSERT_EQ(4, buffer.size);

    char expected[]= {0x00, 0x00, 0x00, 0x00};
    ASSERT_EQ(std::string(expected, 4), std::string(buffer.data, buffer.size));

    rpc_buffer_cleanup(&buffer);
}