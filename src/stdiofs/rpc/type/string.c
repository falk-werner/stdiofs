#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"
#include "stdiofs/rpc/status.h"

#include <stdlib.h>
#include <string.h>

int
rpc_serialize_string(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    char const * value = arg->value;
    size_t length = strlen(value);

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
    char * * value = arg->value;

    uint32_t length;
    int result = rpc_deserialize_raw_u32(buffer, &length);
    if (0 == result)
    {
        char * c = rpc_buffer_read(buffer, length);
        if (NULL != c)
        {
            char * str = malloc(length + 1);
            memcpy(str, c, length);
            str[length] = '\0';
            *value = str;
        }
        else
        {
            result = RPC_BAD_DESERIALIZE_FAILED;
        }
    }

    return result;
}

