#include "rpc/serializer.h"
#include "rpc/buffer.h"
#include "rpc/arg.h"

#include <gtest/gtest.h>

#include <cstring>

TEST(rpc_serializer, empty_method)
{
    rpc_buffer buffer;
    rpc_buffer_init(&buffer, 0);
    rpc_arg args[] = {
        {nullptr, RPC_END, RPC_NONE, nullptr, nullptr}
    };

    int result = rpc_serialize(&buffer, RPC_IN, 42, args);

    ASSERT_EQ(0, result);
    ASSERT_EQ(8, buffer.size);

    char expected_cstr[]={0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 42};
    std::string expected(expected_cstr, 8);
    ASSERT_EQ(expected, std::string(buffer.data, buffer.size));

    rpc_buffer_cleanup(&buffer);
}

TEST(rpc_serializer, fail_to_serialialize_unknown_type)
{
    rpc_buffer buffer;
    rpc_buffer_init(&buffer, 0);
    rpc_arg args[] = {
        {nullptr, RPC_IN, 42, nullptr, nullptr},
        {nullptr, RPC_END, RPC_NONE, nullptr, nullptr}
    };

    int result = rpc_serialize(&buffer, RPC_IN, 42, args);

    ASSERT_NE(0, result);
    rpc_buffer_cleanup(&buffer);
}

TEST(rpc_serializer, int_value)
{
    int value = 0x01020304;
    rpc_buffer buffer;
    rpc_buffer_init(&buffer, 0);
    rpc_arg args[] = {
        {nullptr, RPC_IN, RPC_INT, &value, nullptr},
        {nullptr, RPC_END, RPC_NONE, nullptr, nullptr}
    };

    int result = rpc_serialize(&buffer, RPC_IN, 42, args);

    ASSERT_EQ(0, result);
    ASSERT_EQ(12, buffer.size);

    char expected_cstr[]=
    {
        0x00, 0x00, 0x00, 12,
        0x00, 0x00, 0x00, 42,
        0x01, 0x02, 0x03, 0x04
    };
    std::string expected(expected_cstr, 12);
    ASSERT_EQ(expected, std::string(buffer.data, buffer.size));

    rpc_buffer_cleanup(&buffer);
}

TEST(rpc_serializer, negative_int_value)
{
    int value = -1;
    rpc_buffer buffer;
    rpc_buffer_init(&buffer, 0);
    rpc_arg args[] = {
        {nullptr, RPC_IN, RPC_INT, &value, nullptr},
        {nullptr, RPC_END, RPC_NONE, nullptr, nullptr}
    };

    int result = rpc_serialize(&buffer, RPC_IN, 42, args);

    ASSERT_EQ(0, result);
    ASSERT_EQ(12, buffer.size);

    char expected_cstr[]=
    {
        0x00, 0x00, 0x00, 12,
        0x00, 0x00, 0x00, 42,
        (char) 0xff, (char) 0xff, (char) 0xff, (char) 0xff
    };
    std::string expected(expected_cstr, 12);
    ASSERT_EQ(expected, std::string(buffer.data, buffer.size));

    rpc_buffer_cleanup(&buffer);
}

TEST(rpc_serializer, empty_string)
{
    char const value[] = "";
    rpc_buffer buffer;
    rpc_buffer_init(&buffer, 0);
    rpc_arg args[] = {
        {nullptr, RPC_IN, RPC_STRING, (void*) value, nullptr},
        {nullptr, RPC_END, RPC_NONE, nullptr, nullptr}
    };

    int result = rpc_serialize(&buffer, RPC_IN, 0x0102, args);

    ASSERT_EQ(0, result);
    ASSERT_EQ(12, buffer.size);

    char expected_cstr[]=
    {
        0x00, 0x00, 0x00, 12,
        0x00, 0x00, 0x01, 0x02,
        0x00, 0x00, 0x00, 0x00,
    };
    std::string expected(expected_cstr, 12);
    ASSERT_EQ(expected, std::string(buffer.data, buffer.size));

    rpc_buffer_cleanup(&buffer);
}

TEST(rpc_serializer, string_value)
{
    char const value[] = "foo!";
    rpc_buffer buffer;
    rpc_buffer_init(&buffer, 0);
    rpc_arg args[] = {
        {nullptr, RPC_IN, RPC_STRING, (void*) value, nullptr},
        {nullptr, RPC_END, RPC_NONE, nullptr, nullptr}
    };

    int result = rpc_serialize(&buffer, RPC_IN, 0x0102, args);

    ASSERT_EQ(0, result);
    ASSERT_EQ(16, buffer.size);

    char expected_cstr[]=
    {
        0x00, 0x00, 0x00, 16,
        0x00, 0x00, 0x01, 0x02,
        0x00, 0x00, 0x00, 0x04,
        'f' , 'o' , 'o' , '!'
    };
    std::string expected(expected_cstr, 16);
    ASSERT_EQ(expected, std::string(buffer.data, buffer.size));

    rpc_buffer_cleanup(&buffer);
}

TEST(rpc_serializer, uint64_value)
{
    uint64_t value = 0x0102030405060708;
    rpc_buffer buffer;
    rpc_buffer_init(&buffer, 0);
    rpc_arg args[] = {
        {nullptr, RPC_IN, RPC_UINT64, &value, nullptr},
        {nullptr, RPC_END, RPC_NONE, nullptr, nullptr}
    };

    int result = rpc_serialize(&buffer, RPC_IN, 0x0abc, args);

    ASSERT_EQ(0, result);
    ASSERT_EQ(16, buffer.size);

    char expected_cstr[]=
    {
        0x00, 0x00, 0x00, 16,
        0x00, 0x00, 0x0a, (char)0xbc,
        0x01, 0x02, 0x03, 0x04,
        0x05, 0x06, 0x07, 0x08
    };
    std::string expected(expected_cstr, 16);
    ASSERT_EQ(expected, std::string(buffer.data, buffer.size));

    rpc_buffer_cleanup(&buffer);
}
