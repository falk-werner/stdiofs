#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"
#include "stdiofs/rpc/status.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int
rpc_serialize_dev(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    uint64_t value = (uint64_t) *((dev_t*) arg->value);
    rpc_serialize_raw_u64(buffer, value);

    return 0;
}

int
rpc_deserialize_dev(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    uint64_t value;
    int result = rpc_deserialize_raw_u64(buffer, &value);
    if (0 == result)
    {
        dev_t * offset = arg->value;
        *offset = (dev_t) value;
    }

    return result;
}
