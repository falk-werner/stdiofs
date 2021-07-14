#include "stdiofs/rpc/buffer.h"

#include <gtest/gtest.h>

TEST(rpc_buffer,init)
{
    rpc_buffer buffer; 
    rpc_buffer_init(&buffer, 42);
    
    ASSERT_NE(nullptr, buffer.data);
    ASSERT_EQ(0, buffer.size);
    ASSERT_LE(42, buffer.capacity);

    rpc_buffer_cleanup(&buffer);
}

TEST(rpc_buffer, init_min_capcity)
{
    rpc_buffer buffer; 
    rpc_buffer_init(&buffer, 0);
    
    ASSERT_NE(nullptr, buffer.data);
    ASSERT_EQ(0, buffer.size);
    ASSERT_LT(0, buffer.capacity);

    rpc_buffer_cleanup(&buffer);
}