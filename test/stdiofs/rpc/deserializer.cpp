#include "stdiofs/rpc/deserializer.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"
#include "stdiofs/rpc/types.h"

#include <gtest/gtest.h>

TEST(rpc_deserializer, empty_method)
{
    char data[] = 
    {
        0x00, 0x00, 0x00, 0x08,
        0x00, 0x00, 0x00, 0x2a
    };
    rpc_buffer buffer = {data,8, 8};

    int method_id = rpc_get_method_id(&buffer);
    ASSERT_EQ(42, method_id);
}

TEST(rpc_deserializer, int_value)
{
    char data[] = 
    {
        0x00, 0x00, 0x00, 12,
        0x00, 0x00, 0x00, 0x2a,
        0x01, 0x02, 0x03, 0x04
    };
    rpc_buffer buffer = {data, 12, 12};

    int value = 0;
    rpc_arg args[] = {
        {nullptr, RPC_IN, RPC_INT, &value, nullptr},
        {nullptr, RPC_END, RPC_NONE, nullptr, nullptr}
    };

    int result = rpc_deserialize(&buffer, RPC_IN, args);
    ASSERT_EQ(0, result);
    ASSERT_EQ(0x01020304, value);
}
