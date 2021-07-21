#ifndef FS_PROXY_H
#define FS_PROXY_H

#include "stdiofs/fs/dirbuffer.h"
#include "stdiofs/fs/stringlist.h"

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

struct fs_proxy;
struct rpc;

extern struct fs_proxy *
fs_proxy_create(
    struct rpc * rpc);

extern void
fs_proxy_release(
    struct fs_proxy * proxy);

extern int
fs_proxy_listoperations(
    struct fs_proxy * proxy,
    void * buffer,
    fs_stringlist_add_fn * add);

extern int
fs_proxy_getattr(
    struct fs_proxy * proxy,
    char const * path,
    struct stat * buffer,
    uint64_t file_handle);

extern int
fs_proxy_access(
    struct fs_proxy * proxy,
    char const * path,
    int mask);

extern int
fs_proxy_readlink(
    struct fs_proxy * proxy,
    char const * path,
    char * buffer,
    size_t buffer_size);

extern int
fs_proxy_readdir(
    struct fs_proxy * proxy,
    char const * path,
    void * buffer,
    fs_dirbuffer_add_fn * add,
    off_t offset,
    uint64_t file_handle);

extern int
fs_proxy_mknod(
    struct fs_proxy * proxy,
    char const * path,
    mode_t mode,
    dev_t rdev);

extern int
fs_proxy_mkdir(
    struct fs_proxy * proxy,
    char const * path,
    mode_t mode);

extern int
fs_proxy_unlink(
    struct fs_proxy * proxy,
    char const * path);

extern int
fs_proxy_rmdir(
    struct fs_proxy * proxy,
    char const * path);

extern int
fs_proxy_symlink(
    struct fs_proxy * proxy,
    char const * from,
    char const * to);

extern int
fs_proxy_rename(
    struct fs_proxy * proxy,
    char const * from,
    char const * to,
    unsigned int flags);

extern int
fs_proxy_link(
    struct fs_proxy * proxy,
    char const * from,
    char const * to);

extern int
fs_proxy_chmod(
    struct fs_proxy * proxy,
    char const * path,
    mode_t mode);

extern int
fs_proxy_chown(
    struct fs_proxy * proxy,
    char const * path,
    uid_t uid,
    gid_t gid);

extern int
fs_proxy_truncate(
    struct fs_proxy * proxy,
    char const * path,
    off_t size,
    uint64_t file_handle);

extern int
fs_proxy_create_file(
    struct fs_proxy * proxy,
    char const * path,
    mode_t mode,
    int flags,
    uint64_t * file_handle);

extern int
fs_proxy_open(
    struct fs_proxy * proxy,
    char const * path,
    int flags,
    uint64_t * file_handle);

extern int
fs_proxy_read(
    struct fs_proxy * proxy,
    char const * path,
    char * buffer,
    size_t buffer_size,
    off_t offset,
    uint64_t file_handle);

extern int
fs_proxy_write(
    struct fs_proxy * proxy,
    char const * path,
    char const * buffer,
    size_t buffer_size,
    off_t offset,
    uint64_t file_handle);

extern int
fs_proxy_statfs(
    struct fs_proxy * proxy,
    char const * path,
    struct statvfs * buffer);

extern int
fs_proxy_release_handle(
    struct fs_proxy * proxy,
    char const * path,
    uint64_t file_handle);

extern int
fs_proxy_fsync(
    struct fs_proxy * proxy,
    char const * path,
    int isdatasync,
    uint64_t file_handle);

extern off_t
fs_proxy_lseek(
    struct fs_proxy * proxy,
    char const * path,
    off_t offset,
    int whence,
    uint64_t file_handle);



#ifdef __cplusplus
}
#endif

#endif