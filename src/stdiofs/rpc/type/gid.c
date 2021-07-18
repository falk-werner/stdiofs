#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"
#include "stdiofs/rpc/status.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int
rpc_serialize_gid(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    uint32_t value = (uint32_t) *((gid_t*) arg->value);
    rpc_serialize_raw_u32(buffer, value);

    return 0;
}

int
rpc_deserialize_gid(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    uint32_t value;
    int result = rpc_deserialize_raw_u32(buffer, &value);
    if (0 == result)
    {
        gid_t * mode = arg->value;
        *mode = (gid_t) value;
    }

    return result;
}
