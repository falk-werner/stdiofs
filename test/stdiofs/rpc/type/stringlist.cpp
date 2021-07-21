#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"
#include "stdiofs/rpc/serializer.h"
#include "stdiofs/rpc/deserializer.h"

#include <gtest/gtest.h>

TEST(rpc_stringlist, init_cleanup)
{
    rpc_stringlist list;
    rpc_stringlist_init(&list);
    rpc_stringlist_cleanup(&list);
}

TEST(rpc_stringlist, add)
{
    rpc_stringlist list;
    rpc_stringlist_init(&list);

    rpc_stringlist_add(&list, "foo");
    rpc_stringlist_add(&list, "bar");

    ASSERT_EQ(2, list.size);
    ASSERT_STREQ("foo", list.entries[0]);
    ASSERT_STREQ("bar", list.entries[1]);

    rpc_stringlist_cleanup(&list);
}

TEST(rpc_stringlist, serialize)
{
    rpc_stringlist list;
    rpc_stringlist_init(&list);

    rpc_stringlist_add(&list, "foo");
    rpc_stringlist_add(&list, "bar");

    rpc_buffer buffer;
    rpc_buffer_init(&buffer, 0);

    rpc_arg arg = {"buffer", RPC_IN, RPC_STRINGLIST, &list, NULL };
    int result = rpc_serialize_stringlist(&buffer, &arg);
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
    rpc_stringlist_cleanup(&list);
}

TEST(rpc_stringlist, deserialize)
{
    rpc_stringlist list;
    rpc_stringlist_init(&list);

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

    rpc_arg arg = {"buffer", RPC_IN, RPC_STRINGLIST, &list, (void*) &rpc_stringlist_add};
    int result = rpc_deserialize_stringlist(&buffer, &arg);
    ASSERT_EQ(0, result);

    ASSERT_EQ(2, list.size);
    ASSERT_STREQ("foo", list.entries[0]);
    ASSERT_STREQ("bar", list.entries[1]);

    rpc_stringlist_cleanup(&list);
}