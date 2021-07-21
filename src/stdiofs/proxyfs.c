#define FUSE_USE_VERSION 31

#include "stdiofs/proxyfs.h"
#include "stdiofs/fs/proxy.h"
#include "stdiofs/fs/operations.h"

#include <fuse.h>

#include <stdlib.h>
#include <string.h>

struct proxyfs
{
    struct fs_proxy * proxy;
    struct fuse_operations operations;
};

struct proxyfs_adddir_context
{
    char * buffer;
    fuse_fill_dir_t filler;
};

static int proxyfs_adddir(
    void * buffer,
    char const * name)
{
    struct proxyfs_adddir_context * context = buffer;
    return context->filler(context->buffer, name, NULL, 0, 0);
}

static struct fs_proxy *
proxyfs_get_proxy(void)
{
    struct fuse_context * context = fuse_get_context();
    struct proxyfs * fs = context->private_data;
    return fs->proxy;
}

static void *
proxyfs_init(
    struct fuse_conn_info * connection,
    struct fuse_config * config)
{
    (void) connection;
    config->use_ino = 1;
    config->entry_timeout = 0;
    config->attr_timeout = 0;
    config->negative_timeout = 0;

    struct fuse_context * context = fuse_get_context();
    return context->private_data;
}

static int
proxyfs_getattr(
    char const * path,
    struct stat * buffer,
    struct fuse_file_info * info)
{   
    struct fs_proxy * proxy = proxyfs_get_proxy();

    return fs_proxy_getattr(proxy, path, buffer, (NULL != info) ? info->fh: FS_INVALID_HANDLE);
}

static int
proxyfs_access(
    char const * path,
    int mask)
{
    struct fs_proxy * proxy = proxyfs_get_proxy();

    return fs_proxy_access(proxy, path, mask);
}

static int
proxyfs_readlink(
    char const * path,
    char * buffer,
    size_t buffer_size)
{
    struct fs_proxy * proxy = proxyfs_get_proxy();

    return fs_proxy_readlink(proxy, path, buffer, buffer_size);
}

static int
proxyfs_readdir(
    char const * path,
    void * buffer,
    fuse_fill_dir_t filler,
    off_t offset,
    struct fuse_file_info * info,
    enum fuse_readdir_flags flags)
{
    struct fs_proxy * proxy = proxyfs_get_proxy();

    struct proxyfs_adddir_context context =
    {
        .buffer = buffer,
        .filler = filler
    };

    int result = fs_proxy_readdir(proxy, path, &context, &proxyfs_adddir, offset, info->fh);
    return result;
}

static int
proxyfs_mknod(
    char const * path,
    mode_t mode,
    dev_t rdev)
{
    struct fs_proxy * proxy = proxyfs_get_proxy();

    return fs_proxy_mknod(proxy, path, mode, rdev);
}

static int
proxyfs_mkdir(
    char const * path,
    mode_t mode)
{
    struct fs_proxy * proxy = proxyfs_get_proxy();

    return fs_proxy_mkdir(proxy, path, mode);
}

static int
proxyfs_unlink(
    char const * path)
{
    struct fs_proxy * proxy = proxyfs_get_proxy();

    return fs_proxy_unlink(proxy, path);
}

static int
proxyfs_rmdir(
    char const * path)
{
    struct fs_proxy * proxy = proxyfs_get_proxy();

    return fs_proxy_rmdir(proxy, path);
}

static int
proxyfs_symlink(
    char const * from,
    char const * to)
{
    struct fs_proxy * proxy = proxyfs_get_proxy();

    return fs_proxy_symlink(proxy, from, to);
}

static int
proxyfs_rename(
    char const * from,
    char const * to,
    unsigned int flags)
{
    struct fs_proxy * proxy = proxyfs_get_proxy();

    fs_proxy_rename(proxy, from, to, flags);
}

static int
proxyfs_link(
    char const * from,
    char const * to)
{
    struct fs_proxy * proxy = proxyfs_get_proxy();

    return fs_proxy_link(proxy, from, to);
}

static int
proxyfs_chmod(
    char const * path,
    mode_t mode,
    struct fuse_file_info * info)
{
    (void) info;
    struct fs_proxy * proxy = proxyfs_get_proxy();

    return fs_proxy_chmod(proxy, path, mode);
}

static int
proxyfs_chown(
    char const * path,
    uid_t uid,
    gid_t gid,
    struct fuse_file_info * info)
{
    (void) info;
    struct fs_proxy * proxy = proxyfs_get_proxy();

    return fs_proxy_chown(proxy, path, uid, gid);
}

static int
proxyfs_truncate(
    char const * path,
    off_t size,
    struct fuse_file_info * info)
{
    struct fs_proxy * proxy = proxyfs_get_proxy();

    return fs_proxy_truncate(proxy, path, size, (NULL != info) ? info->fh: FS_INVALID_HANDLE);
}

static int
proxyfs_create_file(
    char const * path,
    mode_t mode,
    struct fuse_file_info * info)
{
    struct fs_proxy * proxy = proxyfs_get_proxy();

    return fs_proxy_create_file(proxy, path, mode, info->flags, &info->fh);
}

static int
proxyfs_open(
    char const * path,
    struct fuse_file_info * info)
{
    struct fs_proxy * proxy = proxyfs_get_proxy();

    return fs_proxy_open(proxy, path, info->flags, &info->fh);
}
static int
proxyfs_read(
    char const * path,
    char * buffer,
    size_t buffer_size,
    off_t offset,
    struct fuse_file_info * info)
{
    struct fs_proxy * proxy = proxyfs_get_proxy();

    return fs_proxy_read(proxy, path, buffer, buffer_size, offset, (NULL != info) ? info->fh : FS_INVALID_HANDLE);
}

static int
proxyfs_write(
    char const * path,
    char const * buffer,
    size_t buffer_size,
    off_t offset,
    struct fuse_file_info * info)
{
    struct fs_proxy * proxy = proxyfs_get_proxy();

    return fs_proxy_write(proxy, path, buffer, buffer_size, offset, (NULL != info) ? info->fh : FS_INVALID_HANDLE);
}

static int
proxyfs_statfs(
    char const * path,
    struct statvfs * buffer)
{
    struct fs_proxy * proxy = proxyfs_get_proxy();

    return fs_proxy_statfs(proxy, path, buffer);
}

static int
proxyfs_release_handle(
    char const * path,
    struct fuse_file_info * info)
{
    struct fs_proxy * proxy = proxyfs_get_proxy();

    return fs_proxy_release_handle(proxy, path, info->fh);
}

static int
proxyfs_fsync(
    char const * path,
    int isdatasync,
    struct fuse_file_info * info)
{
    struct fs_proxy * proxy = proxyfs_get_proxy();

    return fs_proxy_fsync(proxy, path, isdatasync, info->fh);
}

static off_t
proxyfs_lseek(
    char const * path,
    off_t offset,
    int whence,
    struct fuse_file_info * info)
{
    struct fs_proxy * proxy = proxyfs_get_proxy();

    return fs_proxy_lseek(proxy, path, offset, whence, (NULL != info) ? info->fh : FS_INVALID_HANDLE);
}

static int
proxyfs_setoperation(
    void * list,
    char const * name)
{
    struct fuse_operations * operations = list;

    if (0 == strcmp(name, "init"))
    {
        operations->init = &proxyfs_init;
    }
    else if (0 == strcmp(name, "getattr"))
    {
        operations->getattr = &proxyfs_getattr;
    }
    else if (0 == strcmp(name, "access"))
    {
        operations->access = &proxyfs_access;
    }
    else if (0 == strcmp(name, "readlink"))
    {
        operations->readlink = proxyfs_readlink;
    }
    else if (0 == strcmp(name, "readdir"))
    {
        operations->readdir = &proxyfs_readdir;
    }
    else if (0 == strcmp(name, "mknod"))
    {
        operations->mknod = &proxyfs_mknod;
    }
    else if (0 == strcmp(name, "mkdir"))
    {
        operations->mkdir = &proxyfs_mkdir;
    }
    else if (0 == strcmp(name, "unlink"))
    {
        operations->unlink = &proxyfs_unlink;
    }
    else if (0 == strcmp(name, "rmdir"))
    {
        operations->rmdir = proxyfs_rmdir;
    }
    else if (0 == strcmp(name, "symlink"))
    {
        operations->symlink = &proxyfs_symlink;
    }
    else if (0 == strcmp(name, "rename"))
    {
        operations->rename = &proxyfs_rename;
    }
    else if (0 == strcmp(name, "link"))
    {
        operations->link = &proxyfs_link;
    }
    else if (0 == strcmp(name, "chmod"))
    {
        operations->chmod = &proxyfs_chmod;
    }
    else if (0 == strcmp(name, "chown"))
    {
        operations->chown = &proxyfs_chown;
    }
    else if (0 == strcmp(name, "truncate"))
    {
        operations->truncate = &proxyfs_truncate;
    }
    else if (0 == strcmp(name, "create"))
    {
        operations->create = &proxyfs_create_file;
    }
    else if (0 == strcmp(name, "open"))
    {
        operations->open = &proxyfs_open;
    }
    else if (0 == strcmp(name, "read"))
    {
        operations->read = &proxyfs_read;
    }
    else if (0 == strcmp(name, "write"))
    {
        operations->write = &proxyfs_write;
    }
    else if (0 == strcmp(name, "statfs"))
    {
        operations->statfs = &proxyfs_statfs;
    }
    else if (0 == strcmp(name, "release"))
    {
        operations->release = &proxyfs_release_handle;
    }
    else if (0 == strcmp(name, "fsync"))
    {
        operations->fsync = &proxyfs_fsync;
    }
    else if (0 == strcmp(name, "lseek"))
    {
        operations->lseek = &proxyfs_lseek;
    }
}

struct proxyfs *
proxyfs_create(
    struct fs_proxy * proxy)
{
    struct proxyfs * fs = malloc(sizeof(struct proxyfs));
    fs->proxy = proxy;
    memset(&fs->operations, 0, sizeof(struct fuse_operations));

    return fs;
}

void
proxyfs_release(
    struct proxyfs * fs)
{
    free(fs);
}

struct fuse_operations const *
proxyfs_get_operations(
    struct proxyfs * fs)
{
    memset(&fs->operations, 0, sizeof(struct fuse_operations));
    int result = fs_proxy_listoperations(fs->proxy, &fs->operations, 
        proxyfs_setoperation);

    if (0 == result)
    {
        return &fs->operations;
    }
    else
    {
        return NULL;
    }

}
