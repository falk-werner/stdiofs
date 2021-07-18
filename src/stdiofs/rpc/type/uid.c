#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"
#include "stdiofs/rpc/status.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int
rpc_serialize_uid(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    uint32_t value = (uint32_t) *((uid_t*) arg->value);
    rpc_serialize_raw_u32(buffer, value);

    return 0;
}

int
rpc_deserialize_uid(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    uint32_t value;
    int result = rpc_deserialize_raw_u32(buffer, &value);
    if (0 == result)
    {
        uid_t * mode = arg->value;
        *mode = (uid_t) value;
    }

    return result;
}
