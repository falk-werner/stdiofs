#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"
#include "stdiofs/rpc/status.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
rpc_serialize_bytes(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    char const * value = arg->value;
    size_t length = *((size_t*) arg->extra);

    rpc_serialize_raw_u32(buffer, (uint32_t) length);
    if (0 < length)
    {
        rpc_buffer_write(buffer, value, length);
    }

    return 0;
}

int
rpc_deserialize_bytes(
    struct rpc_buffer * buffer,
    struct rpc_arg const *arg)
{
    uint32_t length;
    int result = rpc_deserialize_raw_u32(buffer, &length);
    if (0 == result)
    {
        char * data = rpc_buffer_read(buffer, length);
        if (NULL != data)
        {
            char * bytes = arg->value;
            size_t bytes_size = *((size_t *) arg->extra);
            size_t count = length < bytes_size ? length : bytes_size;
            
            if (count > 0)
            {
                memcpy(bytes, data, count);
            }
        }
        else
        {
            result = RPC_BAD_DESERIALIZE_FAILED;
        }
    }

    return result;
}

