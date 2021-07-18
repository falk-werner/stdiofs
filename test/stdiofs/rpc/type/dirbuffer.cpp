#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"
#include "stdiofs/rpc/serializer.h"
#include "stdiofs/rpc/deserializer.h"

#include <gtest/gtest.h>

TEST(rpc_dirbuffer, init_cleanup)
{
    rpc_dirbuffer dirbuffer;
    rpc_dirbuffer_init(&dirbuffer);
    rpc_dirbuffer_cleanup(&dirbuffer);
}

TEST(rpc_dirbuffer, add)
{
    rpc_dirbuffer dirbuffer;
    rpc_dirbuffer_init(&dirbuffer);

    rpc_dirbuffer_add(&dirbuffer, "foo");
    rpc_dirbuffer_add(&dirbuffer, "bar");

    ASSERT_EQ(2, dirbuffer.size);
    ASSERT_STREQ("foo", dirbuffer.entries[0]);
    ASSERT_STREQ("bar", dirbuffer.entries[1]);

    rpc_dirbuffer_cleanup(&dirbuffer);
}

TEST(rpc_dirbuffer, serialize)
{
    rpc_dirbuffer dirbuffer;
    rpc_dirbuffer_init(&dirbuffer);

    rpc_dirbuffer_add(&dirbuffer, "foo");
    rpc_dirbuffer_add(&dirbuffer, "bar");

    rpc_buffer buffer;
    rpc_buffer_init(&buffer, 0);

    rpc_arg arg = {"buffer", RPC_IN, RPC_DIRBUFFER, &dirbuffer, NULL };
    int result = rpc_serialize_dirbuffer(&buffer, &arg);
    ASSERT_EQ(0, result);

    char expected[] =
    {
        0x00, 0x00, 0x00, 0x02,
        0x00, 0x00, 0x00, 0x04,
        'f', 'o', 'o', '\0',
        0x00, 0x00, 0x00, 0x04,
        'b', 'a', 'r', '\0'
    };

    ASSERT_EQ(20, buffer.size);
    ASSERT_EQ(std::string(expected, 20), std::string(buffer.data, buffer.size));

    rpc_buffer_cleanup(&buffer);
    rpc_dirbuffer_cleanup(&dirbuffer);
}

TEST(rpc_dirbuffer, deserialize)
{
    rpc_dirbuffer dirbuffer;
    rpc_dirbuffer_init(&dirbuffer);

    char contents[] =
    {
        0x00, 0x00, 0x00, 0x02,
        0x00, 0x00, 0x00, 0x04,
        'f', 'o', 'o', '\0',
        0x00, 0x00, 0x00, 0x04,
        'b', 'a', 'r', '\0'
    };
    rpc_buffer buffer;
    buffer.data = contents;
    buffer.size = 0;
    buffer.capacity = 20;

    rpc_arg arg = {"buffer", RPC_IN, RPC_DIRBUFFER, &dirbuffer, (void*) &rpc_dirbuffer_add};
    int result = rpc_deserialize_dirbuffer(&buffer, &arg);
    ASSERT_EQ(0, result);

    ASSERT_EQ(2, dirbuffer.size);
    ASSERT_STREQ("foo", dirbuffer.entries[0]);
    ASSERT_STREQ("bar", dirbuffer.entries[1]);

    rpc_dirbuffer_cleanup(&dirbuffer);
}