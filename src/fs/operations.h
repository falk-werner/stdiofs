#ifndef FS_OPERATIONS_H
#define FS_OPERATIONS_H

#include "fs/dirbuffer.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <unistd.h>

#ifndef __cplusplus
#include <stddef.h>
#include <inttypes.h>
#else
#include <cstddef>
#include <cinttypes>
#endif


#ifdef __cplusplus
extern "C"
{
#endif

#define FS_RENAME_EXCHANGE  1
#define FS_RENAME_NOREPLACE 2

#define FS_INVALID_HANDLE ((uint64_t) -1)

typedef void *
fs_init_fn(
    void * user_data);

typedef void
fs_cleanup_fn(
    void * user_data);

typedef int
fs_getattr_fn(
    void * user_data,
    char const * path,
    struct stat * buffer,
    uint64_t file_handle);

typedef int
fs_access_fn(
    void * user_data,
    char const * path,
    int mask);

typedef int
fs_readlink_fn(
    void * user_data,
    char const * path,
    char * buffer,
    size_t buffer_size);

typedef int
fs_readdir_fn(
    void * user_data,
    char const * path,
    void * buffer,
    fs_dirbuffer_add_fn * add,
    off_t offset,
    uint64_t file_handle);

typedef int
fs_mknod_fn(
    void * user_data,
    char const * path,
    mode_t mode,
    dev_t rdev);

typedef int
fs_mkdir_fn(
    void * user_data,
    char const * path,
    mode_t mode);

typedef int
fs_unlink_fn(
    void * user_data,
    char const * path);

typedef int
fs_rmdir_fn(
    void * user_data,
    char const * path);

typedef int
fs_symlink_fn(
    void * user_data,
    char const * from,
    char const * to);

typedef int
fs_rename_fn(
    void * user_data,
    char const * from,
    char const * to,
    unsigned int flags);

typedef int
fs_link_fn(
    void * user_data,
    char const * from,
    char const * to);

typedef int
fs_chmod_fn(
    void * user_data,
    char const * path,
    mode_t mode);

typedef int
fs_chown_fn(
    void * user_data,
    char const * path,
    uid_t uid,
    gid_t gid);

typedef int
fs_truncate_fn(
    void * user_data,
    char const * path,
    off_t size,
    uint64_t file_handle);

typedef int
fs_create_fn(
    void * user_data,
    char const * path,
    mode_t mode,
    int flags,
    uint64_t * file_handle);

typedef int
fs_open_fn(
    void * user_data,
    char const * path,
    int flags,
    uint64_t * file_handle);

typedef int
fs_read_fn(
    void * user_data,
    char const * path,
    char * buffer,
    size_t buffer_size,
    off_t offset,
    uint64_t file_handle);

typedef int
fs_write_fn(
    void * user_data,
    char const * path,
    char const * buffer,
    size_t buffer_size,
    off_t offset,
    uint64_t file_handle);

typedef int
fs_statfs_fn(
    void * user_data,
    char const * path,
    struct statvfs * buffer);

typedef int
fs_release_fn(
    void * user_data,
    char const * path,
    uint64_t file_handle);

typedef int
fs_fsync_fn(
    void * user_data,
    char const * path,
    int isdatasync,
    uint64_t file_handle);

typedef off_t
fs_lseek_fn(
    void * user_data,
    char const * path,
    off_t offset,
    int whence,
    uint64_t file_handle);

struct fs_operations
{
    fs_init_fn * init;
    fs_cleanup_fn * cleanup;
    fs_getattr_fn * getattr;
    fs_access_fn * access;
    fs_readlink_fn * readlink;
    fs_readdir_fn * readdir;
    fs_mknod_fn * mknod;
    fs_mkdir_fn * mkdir;
    fs_symlink_fn * symlink;
    fs_unlink_fn * unlink;
    fs_rmdir_fn * rmdir;
    fs_rename_fn * rename;
    fs_link_fn * link;
    fs_chmod_fn * chmod;
    fs_chown_fn * chown;
    fs_truncate_fn * truncate;
    fs_open_fn * open;
    fs_create_fn * create;
    fs_read_fn * read;
    fs_write_fn * write;
    fs_statfs_fn * statfs;
    fs_release_fn * release;
    fs_fsync_fn * fsync;
    fs_lseek_fn * lseek;
};



#ifdef __cplusplus
}
#endif


#endif