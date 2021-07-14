#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"

#include <gtest/gtest.h>

TEST(rpc_unknown, fail_to_serialize)
{
    rpc_buffer buffer;
    rpc_buffer_init(&buffer, 0);
    rpc_arg arg = {nullptr, RPC_IN, RPC_NONE, nullptr, nullptr};

    rpc_serialize_fn * serialize = rpc_get_serializer(arg.type);
    int result = serialize(&buffer, &arg);

    ASSERT_EQ(-1, result);
    ASSERT_EQ(0, buffer.size);

    rpc_buffer_cleanup(&buffer);
}