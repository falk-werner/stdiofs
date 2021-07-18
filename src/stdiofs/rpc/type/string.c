#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"
#include "stdiofs/rpc/status.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
rpc_serialize_string(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    char const * value = arg->value;
    size_t length = strlen(value) + 1;

    rpc_serialize_raw_u32(buffer, (uint32_t) length);
    if (0 < length)
    {
        rpc_buffer_write(buffer, value, length);
    }

    return 0;
}

int
rpc_deserialize_string(
    struct rpc_buffer * buffer,
    struct rpc_arg const *arg)
{
    uint32_t length;
    int result = rpc_deserialize_raw_u32(buffer, &length);
    if (0 == result)
    {
        char * str = rpc_buffer_read(buffer, length);
        if (NULL != str)
        {
            if (NULL == arg->extra)
            {
                char * * value = arg->value;
                *value = str;
            }
            else
            {
                char * value = arg->value;
                size_t value_size = *((size_t *) arg->extra);
                snprintf(value, value_size, "%s", str);
            }
        }
        else
        {
            result = RPC_BAD_DESERIALIZE_FAILED;
        }
    }

    return result;
}

