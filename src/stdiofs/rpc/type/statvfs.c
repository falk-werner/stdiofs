#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"

#include <sys/statvfs.h>

#include <string.h>

// the followinf fields are ignored according to libfuse documentation:
// - f_favail
// - f_fsid
// - f_flags
// see https://github.com/libfuse/libfuse/blob/master/include/fuse.h, statfs

int
rpc_serialize_statvfs(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    struct statvfs const * statfs_buffer = arg->value;
    rpc_serialize_raw_u64(buffer, (uint64_t) statfs_buffer->f_bsize);
    rpc_serialize_raw_u64(buffer, (uint64_t) statfs_buffer->f_frsize);
    rpc_serialize_raw_u64(buffer, (uint64_t) statfs_buffer->f_blocks);
    rpc_serialize_raw_u64(buffer, (uint64_t) statfs_buffer->f_bfree);
    rpc_serialize_raw_u64(buffer, (uint64_t) statfs_buffer->f_bavail);
    rpc_serialize_raw_u64(buffer, (uint64_t) statfs_buffer->f_files);
    rpc_serialize_raw_u64(buffer, (uint64_t) statfs_buffer->f_ffree);
    rpc_serialize_raw_u64(buffer, (uint64_t) statfs_buffer->f_namemax);
    return 0;
}

int
rpc_deserialize_statvfs(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    uint64_t fields[8];
    int result = 0;
    for(size_t i = 0; (0 == result) && (i < 8); i++)
    {
        result = rpc_deserialize_raw_u64(buffer, &fields[i]);
    }
    if (0 == result)
    {
        struct statvfs * statfs_buffer = arg->value;
        memset(statfs_buffer, 0, sizeof(struct statvfs));
        statfs_buffer->f_bsize = (unsigned long) fields[0];
        statfs_buffer->f_frsize = (unsigned long) fields[1];
        statfs_buffer->f_blocks = (fsblkcnt_t) fields[2];
        statfs_buffer->f_bfree = (fsblkcnt_t) fields[3];
        statfs_buffer->f_bavail = (fsblkcnt_t) fields[4];
        statfs_buffer->f_files = (fsfilcnt_t) fields[5];
        statfs_buffer->f_ffree = (fsfilcnt_t) fields[6];
        statfs_buffer->f_namemax = (unsigned long) fields[7];
    }

    return result;
}
