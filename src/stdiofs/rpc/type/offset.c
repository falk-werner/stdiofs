#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"
#include "stdiofs/rpc/status.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int
rpc_serialize_offset(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    uint64_t value = (uint64_t) *((off_t*) arg->value);
    rpc_serialize_raw_u64(buffer, value);

    return 0;
}

int
rpc_deserialize_offset(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    uint64_t value;
    int result = rpc_deserialize_raw_u64(buffer, &value);
    if (0 == result)
    {
        off_t * offset = arg->value;
        *offset = (off_t) value;
    }

    return result;
}
