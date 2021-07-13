#include "fs/proxy.h"
#include "fs/stub_intern.h"

#include <stdlib.h>

struct fs_proxy
{
    struct fs_stub * stub;
};

struct fs_proxy *
fs_proxy_create(
    struct fs_stub * stub)
{
    struct fs_proxy * proxy = malloc(sizeof(struct fs_proxy));
    proxy->stub = stub;
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
    return proxy->stub->operations.getattr(proxy->stub->user_data, path, buffer, file_handle);
}

int
fs_proxy_access(
    struct fs_proxy * proxy,
    char const * path,
    int mask)
{
    return proxy->stub->operations.access(proxy->stub->user_data, path, mask);
}

int
fs_proxy_readlink(
    struct fs_proxy * proxy,
    char const * path,
    char * buffer,
    size_t buffer_size)
{
    return proxy->stub->operations.readlink(proxy->stub->user_data, path, buffer, buffer_size);
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
    return proxy->stub->operations.readdir(proxy->stub->user_data, path, buffer, add, offset, file_handle);
}

int
fs_proxy_mknod(
    struct fs_proxy * proxy,
    char const * path,
    mode_t mode,
    dev_t rdev)
{
    return proxy->stub->operations.mknod(proxy->stub->user_data, path, mode, rdev);
}

int
fs_proxy_mkdir(
    struct fs_proxy * proxy,
    char const * path,
    mode_t mode)
{
    return proxy->stub->operations.mkdir(proxy->stub->user_data, path, mode);
}

int
fs_proxy_unlink(
    struct fs_proxy * proxy,
    char const * path)
{
    return proxy->stub->operations.unlink(proxy->stub->user_data, path);
}

int
fs_proxy_rmdir(
    struct fs_proxy * proxy,
    char const * path)
{
    return proxy->stub->operations.rmdir(proxy->stub->user_data, path);
}

int
fs_proxy_symlink(
    struct fs_proxy * proxy,
    char const * from,
    char const * to)
{
    return proxy->stub->operations.symlink(proxy->stub->user_data, from, to);
}

int
fs_proxy_rename(
    struct fs_proxy * proxy,
    char const * from,
    char const * to,
    unsigned int flags)
{
    return proxy->stub->operations.rename(proxy->stub->user_data, from, to, flags);
}

int
fs_proxy_link(
    struct fs_proxy * proxy,
    char const * from,
    char const * to)
{
    return proxy->stub->operations.link(proxy->stub->user_data, from, to);
}

int
fs_proxy_chmod(
    struct fs_proxy * proxy,
    char const * path,
    mode_t mode)
{
    return proxy->stub->operations.chmod(proxy->stub->user_data, path, mode);
}

int
fs_proxy_chown(
    struct fs_proxy * proxy,
    char const * path,
    uid_t uid,
    gid_t gid)
{
    return proxy->stub->operations.chown(proxy->stub->user_data, path, uid, gid);
}

int
fs_proxy_truncate(
    struct fs_proxy * proxy,
    char const * path,
    off_t size,
    uint64_t file_handle)
{
    return proxy->stub->operations.truncate(proxy->stub->user_data, path, size, file_handle);
}

int
fs_proxy_create_file(
    struct fs_proxy * proxy,
    char const * path,
    mode_t mode,
    int flags,
    uint64_t * file_handle)
{
    return proxy->stub->operations.create(proxy->stub->user_data, path, mode, flags, file_handle);
}

int
fs_proxy_open(
    struct fs_proxy * proxy,
    char const * path,
    int flags,
    uint64_t * file_handle)
{
    return proxy->stub->operations.open(proxy->stub->user_data, path, flags, file_handle);
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
    return proxy->stub->operations.read(proxy->stub->user_data, path, buffer, buffer_size, offset, file_handle);
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
    return proxy->stub->operations.write(proxy->stub->user_data, path, buffer, buffer_size, offset, file_handle);
}

int
fs_proxy_statfs(
    struct fs_proxy * proxy,
    char const * path,
    struct statvfs * buffer)
{
    return proxy->stub->operations.statfs(proxy->stub->user_data, path, buffer);
}

int
fs_proxy_release_handle(
    struct fs_proxy * proxy,
    char const * path,
    uint64_t file_handle)
{
    return proxy->stub->operations.release(proxy->stub->user_data, path, file_handle);
}

int
fs_proxy_fsync(
    struct fs_proxy * proxy,
    char const * path,
    int isdatasync,
    uint64_t file_handle)
{
    return proxy->stub->operations.fsync(proxy->stub->user_data, path, isdatasync, file_handle);
}

off_t
fs_proxy_lseek(
    struct fs_proxy * proxy,
    char const * path,
    off_t offset,
    int whence,
    uint64_t file_handle)
{
    return proxy->stub->operations.lseek(proxy->stub->user_data, path, offset, whence, file_handle);
}
