#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string.h>

// st_dev and st_blksize fields are ignored
// according to libfuse documentation
// https://github.com/libfuse/libfuse/blob/master/include/fuse.h, getattr

int
rpc_serialize_stat(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    struct stat const * stat_buffer = arg->value;

    rpc_serialize_raw_u64(buffer, (uint64_t) stat_buffer->st_ino);
    rpc_serialize_raw_u32(buffer, (uint32_t) stat_buffer->st_mode);
    rpc_serialize_raw_u64(buffer, (uint64_t) stat_buffer->st_nlink);
    rpc_serialize_raw_u32(buffer, (uint64_t) stat_buffer->st_uid);
    rpc_serialize_raw_u32(buffer, (uint64_t) stat_buffer->st_gid);
    rpc_serialize_raw_u64(buffer, (uint64_t) stat_buffer->st_rdev);
    rpc_serialize_raw_u64(buffer, (uint64_t) stat_buffer->st_size);
    rpc_serialize_raw_u64(buffer, (uint64_t) stat_buffer->st_blocks);
    rpc_serialize_raw_u32(buffer, (uint32_t) stat_buffer->st_atim.tv_sec);
    rpc_serialize_raw_u32(buffer, (uint32_t) stat_buffer->st_atim.tv_nsec);
    rpc_serialize_raw_u32(buffer, (uint32_t) stat_buffer->st_mtim.tv_sec);
    rpc_serialize_raw_u32(buffer, (uint32_t) stat_buffer->st_mtim.tv_nsec);
    rpc_serialize_raw_u32(buffer, (uint32_t) stat_buffer->st_ctim.tv_sec);
    rpc_serialize_raw_u32(buffer, (uint32_t) stat_buffer->st_ctim.tv_nsec);

    return 0;
}

int
rpc_deserialize_stat(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    uint32_t mode = 0, uid, gid= 0, atime_sec = 0, atime_nsec = 0;
    uint32_t mtime_sec = 0, mtime_nsec = 0, ctime_sec = 0, ctime_nsec= 0;
    uint64_t ino = 0, nlink = 0, rdev = 0, size = 0, blocks = 0;
    int result;

    {
        result = rpc_deserialize_raw_u64(buffer, &ino);
    }

    if (0 == result)
    {
        result = rpc_deserialize_raw_u32(buffer, &mode);
    }

    if (0 == result)
    {
        result = rpc_deserialize_raw_u64(buffer, &nlink);
    }

    if (0 == result)
    {
        result = rpc_deserialize_raw_u32(buffer, &uid);
    }

    if (0 == result)
    {
        result = rpc_deserialize_raw_u32(buffer, &gid);
    }

    if (0 == result)
    {
        result = rpc_deserialize_raw_u64(buffer, &rdev);
    }

    if (0 == result)
    {
        result = rpc_deserialize_raw_u64(buffer, &size);
    }

    if (0 == result)
    {
        result = rpc_deserialize_raw_u64(buffer, &blocks);
    }

    if (0 == result)
    {
        result = rpc_deserialize_raw_u32(buffer, &atime_sec);
    }

    if (0 == result)
    {
        result = rpc_deserialize_raw_u32(buffer, &atime_nsec);
    }

    if (0 == result)
    {
        result = rpc_deserialize_raw_u32(buffer, &mtime_sec);
    }

    if (0 == result)
    {
        result = rpc_deserialize_raw_u32(buffer, &mtime_nsec);
    }

    if (0 == result)
    {
        result = rpc_deserialize_raw_u32(buffer, &ctime_sec);
    }

    if (0 == result)
    {
        result = rpc_deserialize_raw_u32(buffer, &ctime_nsec);
    }

    if (0 == result)
    {
        struct stat * stat_buffer = arg->value;
        memset(stat_buffer, 0, sizeof(struct stat));
        stat_buffer->st_ino = (ino_t) ino;
        stat_buffer->st_mode = (mode_t) mode;
        stat_buffer->st_nlink = (nlink_t) nlink;
        stat_buffer->st_uid = (uid_t) uid;
        stat_buffer->st_gid = (gid_t) gid;
        stat_buffer->st_rdev = (dev_t) rdev;
        stat_buffer->st_size = (off_t) size;
        stat_buffer->st_blocks = (blkcnt_t) blocks;
        stat_buffer->st_atim.tv_sec = (time_t) atime_sec;
        stat_buffer->st_atim.tv_nsec = (long) atime_nsec;
        stat_buffer->st_mtim.tv_sec = (time_t) mtime_sec;
        stat_buffer->st_mtim.tv_nsec = (long) mtime_nsec;
        stat_buffer->st_ctim.tv_sec = (time_t) ctime_sec;
        stat_buffer->st_ctim.tv_nsec = (long) ctime_nsec;
    }

    return result;
}
