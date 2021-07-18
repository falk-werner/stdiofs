#include "stdiofs/fs/proxy.h"
#include "stdiofs/fs/methods.h"

#include "stdiofs/rpc/rpc.h"
#include "stdiofs/rpc/arg.h"
#include "stdiofs/rpc/types.h"

#include <stdlib.h>

struct fs_proxy
{
    struct rpc * rpc;
};

struct fs_proxy *
fs_proxy_create(
    struct rpc * rpc)
{
    struct fs_proxy * proxy = malloc(sizeof(struct fs_proxy));
    proxy->rpc = rpc;
    return proxy;
}

void
fs_proxy_release(
    struct fs_proxy * proxy)
{
    free(proxy);
}


int
fs_proxy_getattr(
    struct fs_proxy * proxy,
    char const * path,
    struct stat * buffer,
    uint64_t file_handle)
{
    int result;

    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING, (void*) path, NULL},
        {"file_handle", RPC_IN , RPC_UINT64, &file_handle, NULL},
        {"result"     , RPC_OUT, RPC_INT   , &result     , NULL},
        {"buffer"     , RPC_OUT, RPC_STAT  , buffer      , NULL},
        {NULL         , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_GETATTR, args);
    if (0 != rc)
    {
        result = -1;
    }
    return result; 
}

int
fs_proxy_access(
    struct fs_proxy * proxy,
    char const * path,
    int mask)
{
    int result;

    struct rpc_arg const args[] =
    {
        {"path"  , RPC_IN , RPC_STRING, (void*) path, NULL},
        {"mask"  , RPC_IN , RPC_INT   , &mask       , NULL},
        {"result", RPC_OUT, RPC_INT   , &result     , NULL},
        {NULL    , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_ACCESS, args);
    if (0 != rc)
    {
        result = -1;
    }
    return result; 
}

int
fs_proxy_readlink(
    struct fs_proxy * proxy,
    char const * path,
    char * buffer,
    size_t buffer_size)
{
    int result;
    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING, (void*) path, NULL},
        {"buffer_size", RPC_IN , RPC_SIZE  , &buffer_size, NULL},
        {"result"     , RPC_OUT, RPC_INT   , &result     , NULL},
        {"buffer"     , RPC_OUT, RPC_STRING, buffer      , &buffer_size},
        {NULL         , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_READLINK, args);
    if (0 != rc)
    {
        result = -1;
    }

    return result; 
}

int
fs_proxy_readdir(
    struct fs_proxy * proxy,
    char const * path,
    void * buffer,
    fs_dirbuffer_add_fn * add,
    off_t offset,
    uint64_t file_handle)
{
    int result;

    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING   , (void*) path, NULL},
        {"offset"     , RPC_IN , RPC_OFFSET   , &offset     , NULL},
        {"file_handle", RPC_IN , RPC_UINT64   , &file_handle, NULL},
        {"result"     , RPC_OUT, RPC_INT      , &result     , NULL},
        {"buffer"     , RPC_OUT, RPC_DIRBUFFER, buffer      , add },
        {NULL         , RPC_END, RPC_NONE     , NULL        , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_READDIR, args);
    if (0 != rc)
    {
        result = -1;
    }
    return result; 
}

int
fs_proxy_mknod(
    struct fs_proxy * proxy,
    char const * path,
    mode_t mode,
    dev_t rdev)
{
    int result;

    struct rpc_arg const args[] =
    {
        {"path"  , RPC_IN , RPC_STRING, (void*) path, NULL},
        {"mode"  , RPC_IN , RPC_MODE  , &mode       , NULL},
        {"rdev"  , RPC_IN , RPC_DEV   , &rdev       , NULL},
        {"result", RPC_OUT, RPC_INT   , &result     , NULL},
        {NULL    , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_MKNOD, args);
    if (0 != rc)
    {
        result = -1;
    }
    return result; 
}

int
fs_proxy_mkdir(
    struct fs_proxy * proxy,
    char const * path,
    mode_t mode)
{
    int result;

    struct rpc_arg const args[] =
    {
        {"path"  , RPC_IN , RPC_STRING, (void*) path, NULL},
        {"mode"  , RPC_IN , RPC_MODE  , &mode       , NULL},
        {"result", RPC_OUT, RPC_INT   , &result     , NULL},
        {NULL    , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_MKDIR, args);
    if (0 != rc)
    {
        result = -1;
    }
    return result; 
}

int
fs_proxy_unlink(
    struct fs_proxy * proxy,
    char const * path)
{
    int result;

    struct rpc_arg const args[] =
    {
        {"path"  , RPC_IN , RPC_STRING, (void*) path, NULL},
        {"result", RPC_OUT, RPC_INT   , &result     , NULL},
        {NULL    , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_UNLINK, args);
    if (0 != rc)
    {
        result = -1;
    }
    return result; 
}

int
fs_proxy_rmdir(
    struct fs_proxy * proxy,
    char const * path)
{
    int result;

    struct rpc_arg const args[] =
    {
        {"path"  , RPC_IN , RPC_STRING, (void*) path, NULL},
        {"result", RPC_OUT, RPC_INT   , &result     , NULL},
        {NULL    , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_RMDIR, args);
    if (0 != rc)
    {
        result = -1;
    }
    return result; 
}

int
fs_proxy_symlink(
    struct fs_proxy * proxy,
    char const * from,
    char const * to)
{
    int result;

    struct rpc_arg const args[] =
    {
        {"from"  , RPC_IN , RPC_STRING, (void*) from, NULL},
        {"to"    , RPC_IN , RPC_STRING, (void*) to  , NULL},
        {"result", RPC_OUT, RPC_INT   , &result     , NULL},
        {NULL    , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_SYMLINK, args);
    if (0 != rc)
    {
        result = -1;
    }
    return result; 
}

int
fs_proxy_rename(
    struct fs_proxy * proxy,
    char const * from,
    char const * to,
    unsigned int flags)
{
    int result;

    struct rpc_arg const args[] =
    {
        {"from"  , RPC_IN , RPC_STRING, (void*) from, NULL},
        {"to"    , RPC_IN , RPC_STRING, (void*) to  , NULL},
        {"flags" , RPC_IN , RPC_INT   , &flags      , NULL},
        {"result", RPC_OUT, RPC_INT   , &result     , NULL},
        {NULL    , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_RENAME, args);
    if (0 != rc)
    {
        result = -1;
    }
    return result; 
}

int
fs_proxy_link(
    struct fs_proxy * proxy,
    char const * from,
    char const * to)
{
    int result;

    struct rpc_arg const args[] =
    {
        {"from"  , RPC_IN , RPC_STRING, (void*) from, NULL},
        {"to"    , RPC_IN , RPC_STRING, (void*) to  , NULL},
        {"result", RPC_OUT, RPC_INT   , &result     , NULL},
        {NULL    , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_LINK, args);
    if (0 != rc)
    {
        result = -1;
    }
    return result; 
}

int
fs_proxy_chmod(
    struct fs_proxy * proxy,
    char const * path,
    mode_t mode)
{
    int result;

    struct rpc_arg const args[] =
    {
        {"path"  , RPC_IN , RPC_STRING, (void*) path, NULL},
        {"mode"  , RPC_IN , RPC_MODE  , &mode       , NULL},
        {"result", RPC_OUT, RPC_INT   , &result     , NULL},
        {NULL    , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_CHMOD, args);
    if (0 != rc)
    {
        result = -1;
    }
    return result; 
}

int
fs_proxy_chown(
    struct fs_proxy * proxy,
    char const * path,
    uid_t uid,
    gid_t gid)
{
    int result;

    struct rpc_arg const args[] =
    {
        {"path"  , RPC_IN , RPC_STRING, (void*) path, NULL},
        {"uid "  , RPC_IN , RPC_UID   , &uid        , NULL},
        {"gid "  , RPC_IN , RPC_GID   , &gid        , NULL},
        {"result", RPC_OUT, RPC_INT   , &result     , NULL},
        {NULL    , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_CHOWN, args);
    if (0 != rc)
    {
        result = -1;
    }
    return result; 
}

int
fs_proxy_truncate(
    struct fs_proxy * proxy,
    char const * path,
    off_t size,
    uint64_t file_handle)
{
    int result;

    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING, (void*) path, NULL},
        {"size"       , RPC_IN , RPC_OFFSET, &size       , NULL},
        {"file_handle", RPC_IN , RPC_UINT64, &file_handle, NULL},
        {"result"     , RPC_OUT, RPC_INT   , &result     , NULL},
        {NULL         , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_TRUNCATE, args);
    if (0 != rc)
    {
        result = -1;
    }
    return result; 
}

int
fs_proxy_create_file(
    struct fs_proxy * proxy,
    char const * path,
    mode_t mode,
    int flags,
    uint64_t * file_handle)
{
    int result;

    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING, (void*) path, NULL},
        {"mode"       , RPC_IN , RPC_MODE  , &mode       , NULL},
        {"flags"      , RPC_IN , RPC_INT   , &flags      , NULL},
        {"result"     , RPC_OUT, RPC_INT   , &result     , NULL},
        {"file_handle", RPC_OUT, RPC_UINT64, file_handle, NULL},
        {NULL         , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_CREATE, args);
    if (0 != rc)
    {
        result = -1;
    }
    return result; 
}

int
fs_proxy_open(
    struct fs_proxy * proxy,
    char const * path,
    int flags,
    uint64_t * file_handle)
{
    int result;

    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING, (void*) path, NULL},
        {"flags"      , RPC_IN , RPC_MODE  , &flags      , NULL},
        {"result"     , RPC_OUT, RPC_INT   , &result     , NULL},
        {"file_handle", RPC_OUT, RPC_UINT64, file_handle, NULL},
        {NULL         , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_OPEN, args);
    if (0 != rc)
    {
        result = -1;
    }
    return result; 
}

int
fs_proxy_read(
    struct fs_proxy * proxy,
    char const * path,
    char * buffer,
    size_t buffer_size,
    off_t offset,
    uint64_t file_handle)
{
    int result;

    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING, (void*) path, NULL},
        {"buffer_size", RPC_IN , RPC_SIZE  , &buffer_size, NULL},
        {"offset"     , RPC_IN , RPC_OFFSET, &offset     , NULL},
        {"file_handle", RPC_IN , RPC_UINT64, &file_handle, NULL},
        {"result"     , RPC_OUT, RPC_INT   , &result     , NULL},
        {"buffer"     , RPC_OUT, RPC_BYTES , buffer      , &buffer_size},
        {NULL         , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_READ, args);
    if (0 != rc)
    {
        result = -1;
    }
    return result; 
}

int
fs_proxy_write(
    struct fs_proxy * proxy,
    char const * path,
    char const * buffer,
    size_t buffer_size,
    off_t offset,
    uint64_t file_handle)
{
    int result;

    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING, (void*) path  , NULL},
        {"buffer"     , RPC_IN , RPC_BYTES , (void*) buffer, &buffer_size},
        {"buffer_size", RPC_IN , RPC_SIZE  , &buffer_size  , NULL},
        {"offset"     , RPC_IN , RPC_OFFSET, &offset       , NULL},
        {"file_handle", RPC_IN , RPC_UINT64, &file_handle  , NULL},
        {"result"     , RPC_OUT, RPC_INT   , &result       , NULL},
        {NULL         , RPC_END, RPC_NONE  , NULL          , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_WRITE, args);
    if (0 != rc)
    {
        result = -1;
    }
    return result; 
}

int
fs_proxy_statfs(
    struct fs_proxy * proxy,
    char const * path,
    struct statvfs * buffer)
{
    int result;

    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING, (void*) path, NULL},
        {"result"     , RPC_OUT, RPC_INT   , &result     , NULL},
        {"buffer"     , RPC_OUT, RPC_STATFS, &buffer     , NULL},
        {NULL         , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_STATFS, args);
    if (0 != rc)
    {
        result = -1;
    }
    return result; 
}

int
fs_proxy_release_handle(
    struct fs_proxy * proxy,
    char const * path,
    uint64_t file_handle)
{
    int result;

    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING, (void*) path, NULL},
        {"file_handle", RPC_IN , RPC_UINT64, &file_handle, NULL},
        {"result"     , RPC_OUT, RPC_INT   , &result     , NULL},
        {NULL         , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_RELEASE, args);
    if (0 != rc)
    {
        result = -1;
    }
    return result; 
}

int
fs_proxy_fsync(
    struct fs_proxy * proxy,
    char const * path,
    int isdatasync,
    uint64_t file_handle)
{
    int result;

    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING, (void*) path, NULL},
        {"isdatasync" , RPC_IN , RPC_INT   , &isdatasync , NULL},
        {"file_handle", RPC_IN , RPC_UINT64, &file_handle, NULL},
        {"result"     , RPC_OUT, RPC_INT   , &result     , NULL},
        {NULL         , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_FSYNC, args);
    if (0 != rc)
    {
        result = -1;
    }
    return result; 
}

off_t
fs_proxy_lseek(
    struct fs_proxy * proxy,
    char const * path,
    off_t offset,
    int whence,
    uint64_t file_handle)
{
    off_t result;

    struct rpc_arg const args[] =
    {
        {"path"       , RPC_IN , RPC_STRING, (void*) path, NULL},
        {"offset"     , RPC_IN , RPC_OFFSET, &offset     , NULL},
        {"whence"     , RPC_IN , RPC_INT   , &whence     , NULL},
        {"file_handle", RPC_IN , RPC_UINT64, &file_handle, NULL},
        {"result"     , RPC_OUT, RPC_OFFSET, &result     , NULL},
        {NULL         , RPC_END, RPC_NONE  , NULL        , NULL}
    };

    int rc = rpc_invoke(proxy->rpc, FS_METHOD_LSEEK, args);
    if (0 != rc)
    {
        result = -1;
    }
    return result; 
}
