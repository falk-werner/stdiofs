#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"

#include <string.h>

int
rpc_serialize_string(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    char const * value = arg->value;
    size_t length = strlen(value);

    rpc_serialize_raw_size(buffer, length);
    if (0 < length)
    {
        rpc_buffer_write(buffer, value, length);
    }

    return 0;
}

