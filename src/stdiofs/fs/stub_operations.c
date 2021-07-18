#include "stdiofs/fs/stub_operations.h"
#include "stdiofs/fs/stub_intern.h"
#include "stdiofs/fs/methods.h"

#include "stdiofs/rpc/arg.h"
#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/deserializer.h"
#include "stdiofs/rpc/serializer.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <unistd.h>

#include <stdlib.h>
#include <stddef.h>
#include <inttypes.h>

#include <stdio.h>

static int
fs_stub_unknown_method(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    (void) stub;
    (void) buffer;

    return -1;
}

static int
fs_stub_getattr(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * path;
    struct stat stat_buffer;
    uint64_t file_handle;
    int op_result;

    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING, &path       , NULL},
        {"file_handle", RPC_IN , RPC_UINT64, &file_handle, NULL},
        {"result"     , RPC_OUT, RPC_INT   , &op_result  , NULL},
        {"buffer"     , RPC_OUT, RPC_STAT  , &stat_buffer, NULL},
        {NULL         , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, args);
    if (0 == result)
    {
        op_result = stub->operations.getattr(stub->user_data, 
            path, &stat_buffer, file_handle);

        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_GETATTR, args);
    }

    printf("getattr: %d\n", result);
    return result;
}

static int
fs_stub_access(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * path;
    int mask;
    int op_result;

    struct rpc_arg const args[] =
    {
        {"path"  , RPC_IN , RPC_STRING, &path     , NULL},
        {"mask"  , RPC_IN , RPC_INT   , &mask     , NULL},
        {"result", RPC_OUT, RPC_INT   , &op_result, NULL},
        {NULL    , RPC_END, RPC_NONE  , NULL      , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, args);
    if (0 == result)
    {
        op_result = stub->operations.access(stub->user_data, 
            path, mask);

        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_ACCESS, args);
    }

    printf("access: %d\n", result);
    return result;
}

static int
fs_stub_readlink(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    return -1;
}

static int
fs_stub_readdir(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    return -1;
}

static int
fs_stub_mknod(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    return -1;
}

static int
fs_stub_mkdir(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    return -1;
}

static int
fs_stub_symlink(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    return -1;
}

static int
fs_stub_unlink(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    return -1;
}

static int
fs_stub_rmdir(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    return -1;
}

static int
fs_stub_rename(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    return -1;
}

static int
fs_stub_link(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    return -1;
}

static int
fs_stub_chmod(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    return -1;
}

static int
fs_stub_chown(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    return -1;
}

static int
fs_stub_truncate(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    return -1;
}

static int
fs_stub_create_file(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    return -1;
}

static int
fs_stub_open(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    return -1;
}

static int
fs_stub_read(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    return -1;
}

static int
fs_stub_write(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    return -1;
}

static int
fs_stub_statfs(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    return -1;
}

static int
fs_stub_release_handle(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    return -1;
}

static int
fs_stub_fsync(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    return -1;
}

static int
fs_stub_lseek(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    return -1;
}


fs_stub_operation_fn *
fs_stub_get_operation(
    int method_id)
{
    switch (method_id)
    {
        case FS_METHOD_GETATTR:
            return &fs_stub_getattr;
        case FS_METHOD_ACCESS:
            return &fs_stub_access;
        case FS_METHOD_READLINK:
            return &fs_stub_readlink;
        case FS_METHOD_READDIR:
            return &fs_stub_readdir;
        case FS_METHOD_MKNOD:
            return &fs_stub_mkdir;
        case FS_METHOD_MKDIR:
            return &fs_stub_mkdir;
        case FS_METHOD_SYMLINK:
            return &fs_stub_symlink;
        case FS_METHOD_UNLINK:
            return &fs_stub_unlink;
        case FS_METHOD_RMDIR:
            return &fs_stub_rmdir;
        case FS_METHOD_RENAME:
            return &fs_stub_rename;
        case FS_METHOD_LINK:
            return &fs_stub_link;
        case FS_METHOD_CHMOD:
            return &fs_stub_chmod;
        case FS_METHOD_CHOWN:
            return &fs_stub_chown;
        case FS_METHOD_TRUNCATE:
            return &fs_stub_truncate;
        case FS_METHOD_CREATE:
            return &fs_stub_create_file;
        case FS_METHOD_OPEN:
            return &fs_stub_open;
        case FS_METHOD_READ:
            return &fs_stub_read;
        case FS_METHOD_WRITE:
            return &fs_stub_write;
        case FS_METHOD_STATFS:
            return &fs_stub_statfs;
        case FS_METHOD_RELEASE:
            return &fs_stub_release_handle;
        case FS_METHOD_FSYNC:
            return &fs_stub_fsync;
        case FS_METHOD_LSEEK:
            return &fs_stub_lseek;
        default:
            return &fs_stub_unknown_method;
    }
}
