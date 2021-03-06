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

static void
fs_stub_addifset(
    struct rpc_stringlist * list,
    void * pointer,
    char const * value)
{
    if (NULL != pointer)
    {
        rpc_stringlist_add(list, value);
    }
}

static int
fs_stub_listoperations(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    struct rpc_stringlist list;
    rpc_stringlist_init(&list);

    fs_stub_addifset(&list, stub->operations.getattr , "getattr");
    fs_stub_addifset(&list, stub->operations.access  , "access");
    fs_stub_addifset(&list, stub->operations.readlink, "readlink");
    fs_stub_addifset(&list, stub->operations.readdir , "readdir");
    fs_stub_addifset(&list, stub->operations.mknod   , "mknod");
    fs_stub_addifset(&list, stub->operations.mkdir   , "mkdir");
    fs_stub_addifset(&list, stub->operations.symlink , "symlink");
    fs_stub_addifset(&list, stub->operations.unlink  , "unlink");
    fs_stub_addifset(&list, stub->operations.rmdir   , "rmdir");
    fs_stub_addifset(&list, stub->operations.rename  , "rename");
    fs_stub_addifset(&list, stub->operations.link    , "link");
    fs_stub_addifset(&list, stub->operations.chmod   , "chmod");
    fs_stub_addifset(&list, stub->operations.chown   , "chown");
    fs_stub_addifset(&list, stub->operations.truncate, "truncate");
    fs_stub_addifset(&list, stub->operations.create  , "create");
    fs_stub_addifset(&list, stub->operations.open    , "open");
    fs_stub_addifset(&list, stub->operations.read    , "read");
    fs_stub_addifset(&list, stub->operations.write   , "write");
    fs_stub_addifset(&list, stub->operations.statfs  , "statfs");
    fs_stub_addifset(&list, stub->operations.release , "release");
    fs_stub_addifset(&list, stub->operations.fsync   , "fsync");
    fs_stub_addifset(&list, stub->operations.lseek   , "lseek");

    int op_result = 0;
    struct rpc_arg const args[] =
    {
        {"result"     , RPC_OUT, RPC_INT       , &op_result  , NULL},
        {"operations" , RPC_OUT, RPC_STRINGLIST, &list, NULL},
        {NULL         , RPC_END, RPC_NONE      , NULL        , NULL}
    };

    return rpc_serialize(buffer, RPC_OUT, FS_METHOD_LISTOPERATIONS, args);
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

    return result;
}

static int
fs_stub_readlink(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * path;
    size_t buffer_size;

    struct rpc_arg const inargs[] =
    {
        {"path"       , RPC_IN , RPC_STRING, &path       , NULL},
        {"buffer_size", RPC_IN , RPC_SIZE  , &buffer_size, NULL},
        {NULL         , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, inargs);
    if (0 == result)
    {
        char * name_buffer = malloc(buffer_size);
        int op_result = stub->operations.readlink(stub->user_data, 
            path, name_buffer, buffer_size);

        struct rpc_arg const outargs[] =
        {
            {"result"     , RPC_OUT, RPC_INT   , &op_result  , NULL},
            {"buffer"     , RPC_OUT, RPC_STRING, name_buffer , NULL},
            {NULL         , RPC_END, RPC_NONE  , NULL        , NULL}
        };

        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_READLINK, outargs);
        free(name_buffer);
    }

    return result;
}

static int
fs_stub_readdir(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * path;
    off_t offset;
    uint64_t file_handle;
    int op_result;

    struct rpc_stringlist list;
    rpc_stringlist_init(&list);

    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING    , &path       , NULL},
        {"offset"     , RPC_IN , RPC_OFFSET    , &offset     , NULL},
        {"file_handle", RPC_IN , RPC_UINT64    , &file_handle, NULL},
        {"result"     , RPC_OUT, RPC_INT       , &op_result  , NULL},
        {"buffer"     , RPC_OUT, RPC_STRINGLIST, &list       , NULL},
        {NULL         , RPC_END, RPC_NONE      , NULL        , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, args);
    if (0 == result)
    {
        op_result = stub->operations.readdir(stub->user_data, 
            path, &list, &rpc_stringlist_add, offset, file_handle);

        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_READDIR, args);
    }

    rpc_stringlist_cleanup(&list);
    return result;
}

static int
fs_stub_mknod(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * path;
    mode_t mode;
    dev_t rdev;
    int op_result;

    struct rpc_arg const args[] =
    {
        {"path"  , RPC_IN , RPC_STRING, &path     , NULL},
        {"mode"  , RPC_IN , RPC_MODE  , &mode     , NULL},
        {"rdev"  , RPC_IN , RPC_DEV   , &rdev     , NULL},
        {"result", RPC_OUT, RPC_INT   , &op_result, NULL},
        {NULL    , RPC_END, RPC_NONE  , NULL      , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, args);
    if (0 == result)
    {
        op_result = stub->operations.mknod(stub->user_data, 
            path, mode, rdev);

        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_MKNOD, args);
    }

    return result;
}

static int
fs_stub_mkdir(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * path;
    mode_t mode;
    int op_result;

    struct rpc_arg const args[] =
    {
        {"path"  , RPC_IN , RPC_STRING, &path     , NULL},
        {"mode"  , RPC_IN , RPC_MODE  , &mode     , NULL},
        {"result", RPC_OUT, RPC_INT   , &op_result, NULL},
        {NULL    , RPC_END, RPC_NONE  , NULL      , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, args);
    if (0 == result)
    {
        op_result = stub->operations.mkdir(stub->user_data, 
            path, mode);

        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_MKDIR, args);
    }

    return result;
}

static int
fs_stub_symlink(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * from;
    char * to;
    int op_result;

    struct rpc_arg const args[] =
    {
        {"from"  , RPC_IN , RPC_STRING, &from     , NULL},
        {"to"    , RPC_IN , RPC_STRING, &to       , NULL},
        {"result", RPC_OUT, RPC_INT   , &op_result, NULL},
        {NULL    , RPC_END, RPC_NONE  , NULL      , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, args);
    if (0 == result)
    {
        op_result = stub->operations.symlink(stub->user_data, 
            from, to);

        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_SYMLINK, args);
    }

    return result;
}

static int
fs_stub_unlink(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * path;
    int op_result;

    struct rpc_arg const args[] =
    {
        {"path"  , RPC_IN , RPC_STRING, &path     , NULL},
        {"result", RPC_OUT, RPC_INT   , &op_result, NULL},
        {NULL    , RPC_END, RPC_NONE  , NULL      , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, args);
    if (0 == result)
    {
        op_result = stub->operations.unlink(stub->user_data, path);
        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_UNLINK, args);
    }

    return result;
}

static int
fs_stub_rmdir(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * path;
    int op_result;

    struct rpc_arg const args[] =
    {
        {"path"  , RPC_IN , RPC_STRING, &path     , NULL},
        {"result", RPC_OUT, RPC_INT   , &op_result, NULL},
        {NULL    , RPC_END, RPC_NONE  , NULL      , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, args);
    if (0 == result)
    {
        op_result = stub->operations.rmdir(stub->user_data, path);
        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_RMDIR, args);
    }

    return result;
}

static int
fs_stub_rename(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * from;
    char * to;
    int flags;
    int op_result;

    struct rpc_arg const args[] =
    {
        {"from"  , RPC_IN , RPC_STRING, &from     , NULL},
        {"to"    , RPC_IN , RPC_STRING, &to       , NULL},
        {"flags" , RPC_IN , RPC_INT   , &flags    , NULL},
        {"result", RPC_OUT, RPC_INT   , &op_result, NULL},
        {NULL    , RPC_END, RPC_NONE  , NULL      , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, args);
    if (0 == result)
    {
        op_result = stub->operations.rename(stub->user_data, 
            from, to, flags);

        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_RENAME, args);
    }

    return result;
}

static int
fs_stub_link(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * from;
    char * to;
    int op_result;

    struct rpc_arg const args[] =
    {
        {"from"  , RPC_IN , RPC_STRING, &from     , NULL},
        {"to"    , RPC_IN , RPC_STRING, &to       , NULL},
        {"result", RPC_OUT, RPC_INT   , &op_result, NULL},
        {NULL    , RPC_END, RPC_NONE  , NULL      , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, args);
    if (0 == result)
    {
        op_result = stub->operations.link(stub->user_data, 
            from, to);

        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_LINK, args);
    }

    return result;
}

static int
fs_stub_chmod(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * path;
    mode_t mode;
    int op_result;

    struct rpc_arg const args[] =
    {
        {"path"  , RPC_IN , RPC_STRING, &path     , NULL},
        {"mode"  , RPC_IN , RPC_MODE  , &mode     , NULL},
        {"result", RPC_OUT, RPC_INT   , &op_result, NULL},
        {NULL    , RPC_END, RPC_NONE  , NULL      , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, args);
    if (0 == result)
    {
        op_result = stub->operations.chmod(stub->user_data, path, mode);
        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_CHMOD, args);
    }

    return result;
}

static int
fs_stub_chown(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * path;
    uid_t uid;
    gid_t gid;
    int op_result;

    struct rpc_arg const args[] =
    {
        {"path"  , RPC_IN , RPC_STRING, &path     , NULL},
        {"uid "  , RPC_IN , RPC_UID   , &uid      , NULL},
        {"gid "  , RPC_IN , RPC_GID   , &gid      , NULL},
        {"result", RPC_OUT, RPC_INT   , &op_result, NULL},
        {NULL    , RPC_END, RPC_NONE  , NULL      , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, args);
    if (0 == result)
    {
        op_result = stub->operations.chown(stub->user_data, 
            path, uid, gid);
        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_CHOWN, args);
    }

    return result;
}

static int
fs_stub_truncate(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * path;
    off_t size;
    uint64_t file_handle;
    int op_result;

    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING, &path       , NULL},
        {"size"       , RPC_IN , RPC_OFFSET, &size       , NULL},
        {"file_handle", RPC_IN , RPC_UINT64, &file_handle, NULL},
        {"result"     , RPC_OUT, RPC_INT   , &op_result  , NULL},
        {NULL    , RPC_END, RPC_NONE  , NULL      , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, args);
    if (0 == result)
    {
        op_result = stub->operations.truncate(stub->user_data, 
            path, size, file_handle);
        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_TRUNCATE, args);
    }

    return result;
}

static int
fs_stub_create_file(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * path;
    mode_t mode;
    int flags;
    uint64_t file_handle;
    int op_result;

    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING, &path       , NULL},
        {"mode"       , RPC_IN , RPC_MODE  , &mode       , NULL},
        {"flags"      , RPC_IN , RPC_INT   , &flags      , NULL},
        {"result"     , RPC_OUT, RPC_INT   , &op_result  , NULL},
        {"file_handle", RPC_OUT, RPC_UINT64, &file_handle, NULL},
        {NULL         , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, args);
    if (0 == result)
    {
        op_result = stub->operations.create(stub->user_data, 
            path, mode, flags, &file_handle);
        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_CREATE, args);
    }

    return result;
}

static int
fs_stub_open(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * path;
    int flags;
    uint64_t file_handle;
    int op_result;

    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING, &path       , NULL},
        {"flags"      , RPC_IN , RPC_INT   , &flags      , NULL},
        {"result"     , RPC_OUT, RPC_INT   , &op_result  , NULL},
        {"file_handle", RPC_OUT, RPC_UINT64, &file_handle, NULL},
        {NULL         , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, args);
    if (0 == result)
    {
        op_result = stub->operations.open(stub->user_data, 
            path, flags, &file_handle);
        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_OPEN, args);
    }

    return result;
}

static int
fs_stub_read(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * path;
    size_t buffer_size;
    off_t offset;
    uint64_t file_handle;

    struct rpc_arg const inargs[] =
    {
        {"path"       , RPC_IN , RPC_STRING, &path       , NULL},
        {"buffer_size", RPC_IN , RPC_SIZE  , &buffer_size, NULL},
        {"offset"     , RPC_IN , RPC_OFFSET, &offset     , NULL},
        {"file_handle", RPC_IN , RPC_UINT64, &file_handle, NULL},
        {NULL         , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, inargs);
    if (0 == result)
    {
        char * read_buffer = malloc(buffer_size);

        int op_result = stub->operations.read(stub->user_data, 
            path, read_buffer, buffer_size, offset, file_handle);

        size_t length = (op_result > 0) ? (size_t) op_result : 0;
        struct rpc_arg const outargs[] =
        {
            {"result"     , RPC_OUT, RPC_INT   , &op_result  , NULL},
            {"buffer"     , RPC_OUT, RPC_BYTES , read_buffer , &length},
            {NULL         , RPC_END, RPC_NONE  , NULL        , NULL}
        };

        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_READ, outargs);
        free(read_buffer);
    }

    return result;
}

static int
fs_stub_write(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * path;
    char * write_buffer;
    size_t buffer_size = (size_t) -1;
    off_t offset;
    uint64_t file_handle;
    int op_result;

    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING, &path       , NULL},
        {"buffer"     , RPC_IN , RPC_BYTES , &write_buffer, &buffer_size},
        {"offset"     , RPC_IN , RPC_OFFSET, &offset     , NULL},
        {"file_handle", RPC_IN , RPC_UINT64, &file_handle, NULL},
        {"result"     , RPC_OUT, RPC_INT   , &op_result  , NULL},
        {NULL         , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, args);
    if (0 == result)
    {
        op_result = stub->operations.write(stub->user_data, 
            path, write_buffer, buffer_size, offset, file_handle);

        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_WRITE, args);
    }

    return result;
}

static int
fs_stub_statfs(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * path;
    struct statvfs stat_buffer;
    int op_result;

    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING , &path       , NULL},
        {"result"     , RPC_OUT, RPC_INT    , &op_result  , NULL},
        {"buffer"     , RPC_OUT, RPC_STATVFS, &stat_buffer, NULL},
        {NULL         , RPC_END, RPC_NONE   , NULL        , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, args);
    if (0 == result)
    {
        op_result = stub->operations.statfs(stub->user_data, 
            path, &stat_buffer);

        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_STATFS, args);
    }

    return result;
}

static int
fs_stub_release_handle(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * path;
    uint64_t file_handle;
    int op_result;

    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING, &path       , NULL},
        {"file_handle", RPC_IN , RPC_UINT64, &file_handle, NULL},
        {"result"     , RPC_OUT, RPC_INT   , &op_result  , NULL},
        {NULL         , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, args);
    if (0 == result)
    {
        op_result = stub->operations.release(stub->user_data, 
            path, file_handle);
        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_RELEASE, args);
    }

    return result;
}

static int
fs_stub_fsync(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * path;
    int isdatasync;
    uint64_t file_handle;
    int op_result;

    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING, &path       , NULL},
        {"isdatasync" , RPC_IN , RPC_INT   , &isdatasync , NULL},
        {"file_handle", RPC_IN , RPC_UINT64, &file_handle, NULL},
        {"result"     , RPC_OUT, RPC_INT   , &op_result  , NULL},
        {NULL         , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, args);
    if (0 == result)
    {
        op_result = stub->operations.fsync(stub->user_data, 
            path, isdatasync, file_handle);
        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_FSYNC, args);
    }

    return result;
}

static int
fs_stub_lseek(
    struct fs_stub * stub,
    struct rpc_buffer * buffer)
{
    char * path;
    off_t offset;
    int whence;
    uint64_t file_handle;
    off_t op_result;

    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING, &path       , NULL},
        {"offset"     , RPC_IN , RPC_OFFSET, &offset     , NULL},
        {"whence"     , RPC_IN , RPC_INT   , &whence     , NULL},
        {"file_handle", RPC_IN , RPC_UINT64, &file_handle, NULL},
        {"result"     , RPC_OUT, RPC_OFFSET, &op_result  , NULL},
        {NULL         , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int result = rpc_deserialize(buffer, RPC_IN, args);
    if (0 == result)
    {
        op_result = stub->operations.lseek(stub->user_data, 
            path, offset, whence, file_handle);
        result = rpc_serialize(buffer, RPC_OUT, FS_METHOD_LSEEK, args);
    }

    return result;
}


fs_stub_operation_fn *
fs_stub_get_operation(
    int method_id)
{
    switch (method_id)
    {
        case FS_METHOD_LISTOPERATIONS:
            return &fs_stub_listoperations;
        case FS_METHOD_GETATTR:
            return &fs_stub_getattr;
        case FS_METHOD_ACCESS:
            return &fs_stub_access;
        case FS_METHOD_READLINK:
            return &fs_stub_readlink;
        case FS_METHOD_READDIR:
            return &fs_stub_readdir;
        case FS_METHOD_MKNOD:
            return &fs_stub_mknod;
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
