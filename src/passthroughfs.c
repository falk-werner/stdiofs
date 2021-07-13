#define _GNU_SOURCE

#include "passthroughfs.h"
#include "fs/operations.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct passthroughfs
{
    char * working_dir;
};

static char *
passthroughfs_get_effective_path(
    void * user_data,
    char const * path)
{
    struct passthroughfs * fs = user_data;

    char * result;
    asprintf(&result, "%s%s", fs->working_dir, path);
    return result;

}

static void *
passthroughfs_init(
    void * user_data)
{
    return user_data;
}

static void
passthroughfs_cleanup(
    void * user_data)
{
    (void) user_data;
}

static int
passthroughfs_getattr(
    void * user_data,
    char const * path,
    struct stat * buffer,
    uint64_t file_handle)
{
    (void) file_handle;
    char * effective_path = passthroughfs_get_effective_path(user_data, path);
    printf("getattr: path=%s\n", effective_path);

    int result = lstat(effective_path, buffer);
    if (-1 == result)
    {
        result = -errno;
    }

    free(effective_path);
    return result;
}

static int
passthroughfs_access(
    void * user_data,
    char const * path,
    int mask)
{
    char * effective_path = passthroughfs_get_effective_path(user_data, path);
    printf("access: path=%s, mask=0x%04x\n", effective_path, mask);

    int result = access(effective_path, mask);
    if (-1 == result)
    {
        result = -errno;
    }

    free(effective_path);
    return result;    
}

static int
passthroughfs_readlink(
    void * user_data,
    char const * path,
    char * buffer,
    size_t buffer_size)
{
    char * effective_path = passthroughfs_get_effective_path(user_data, path);
    printf("readlink: path=%s\n", effective_path);

    int result = readlink(effective_path, buffer, buffer_size - 1);
    if (0 <= result)
    {
        buffer[result] = '\0';
        result = 0;
    }
    else
    {
        result = -errno;
    }
    
    free(effective_path);
    return result;
}

static int
passthroughfs_readdir(
    void * user_data,
    char const * path,
    void * buffer,
    fs_dirbuffer_add_fn * add,
    off_t offset,
    uint64_t file_handle)
{
    (void) offset;
    (void) file_handle;
    int result = 0;

    char * effective_path = passthroughfs_get_effective_path(user_data, path);
    printf("readdir: path=%s\n", effective_path);
    DIR * dir = opendir(effective_path);
    if (NULL != dir)
    {
        struct dirent * entry = readdir(dir);
        while (NULL != entry)
        {
            int is_full = add(buffer, entry->d_name);
            if (is_full)
            {
                break;
            }

            entry = readdir(dir);
        }
        closedir(dir);
    }
    else
    {
        result = -errno;
    }

    free(effective_path);
    return result;
}

static int
passthroughfs_mknod(
    void * user_data,
    char const * path,
    mode_t mode,
    dev_t rdev)
{
   char * effective_path = passthroughfs_get_effective_path(user_data, path);
   printf("mknod: path=%s\n", effective_path);
   int result = 0;

    if (S_ISREG(mode))
    {
        result = open(effective_path, O_CREAT | O_EXCL | O_WRONLY, mode);
        if (0 <= result)
        {
            close(result);
        }
    }
    else if (S_ISDIR(mode))
    {
        result = mkdir(effective_path, mode);
    }
    else
    {
        result -1;
    }


    free(effective_path);
    return result;
}

static int
passthroughfs_mkdir(
    void * user_data,
    char const * path,
    mode_t mode)
{
    char * effective_path = passthroughfs_get_effective_path(user_data, path);
    printf("mkdir: path=%s\n", effective_path);

    int result = mkdir(effective_path, mode);
    if (-1 == result)
    {
        result = -errno;
    }

    free(effective_path);
    return result;
}

static int
passthroughfs_unlink(
    void * user_data,
    char const * path)
{
    char * effective_path = passthroughfs_get_effective_path(user_data, path);
    printf("unlink: path=%s\n", effective_path);

    int result = unlink(effective_path);
    if (-1 == result)
    {
        result = -errno;
    }

    free(effective_path);
    return result;
}

static int
passthroughfs_rmdir(
    void * user_data,
    char const * path)
{
    char * effective_path = passthroughfs_get_effective_path(user_data, path);
    printf("rmdir: path=%s\n", effective_path);

    int result = rmdir(effective_path);
    if (-1 == result)
    {
        result = -errno;
    }

    free(effective_path);
    return result;
}

static int
passthroughfs_symlink(
    void * user_data,
    char const * from,
    char const * to)
{
    char * effective_from = ('/' == from[0]) ? passthroughfs_get_effective_path(user_data, from) : strdup(from);
    char * effective_to = ('/' == to[0]) ? passthroughfs_get_effective_path(user_data, to) : strdup(to);
    printf("symlink: from=%s, to=%s\n", effective_from, effective_to);

    int result = symlink(effective_from, effective_to);
    if (-1 == result)
    {
        result = -errno;
    }

    free(effective_from);
    free(effective_to);
    return result;
}

static int
passthroughfs_rename(
    void * user_data,
    char const * from,
    char const * to,
    unsigned int flags)
{
    if (flags != 0)
    {
        return -EINVAL;
    }

    char * effective_from = ('/' == from[0]) ? passthroughfs_get_effective_path(user_data, from) : strdup(from);
    char * effective_to = ('/' == to[0]) ? passthroughfs_get_effective_path(user_data, to) : strdup(to);
    printf("rename: from=%s, to=%s\n", effective_from, effective_to);

    int result = rename(effective_from, effective_to);
    if (-1 == result)
    {
        result = -errno;
    }

    free(effective_from);
    free(effective_to);
    return result;
}

static int
passthroughfs_link(
    void * user_data,
    char const * from,
    char const * to)
{
    char * effective_from = ('/' == from[0]) ? passthroughfs_get_effective_path(user_data, from) : strdup(from);
    char * effective_to = ('/' == to[0]) ? passthroughfs_get_effective_path(user_data, to) : strdup(to);
    printf("link: from=%s, to=%s\n", effective_from, effective_to);

    int result = link(effective_from, effective_to);
    if (-1 == result)
    {
        result = -errno;
    }

    free(effective_from);
    free(effective_to);
    return result;
}

static int
passthroughfs_chmod(
    void * user_data,
    char const * path,
    mode_t mode)
{
    char * effective_path = passthroughfs_get_effective_path(user_data, path);
    printf("chmod: path=%s\n", effective_path);

    int result = chmod(effective_path, mode);
    if (-1 == result)
    {
        result = -errno;
    }

    free(effective_path);
    return result;
}

static int
passthroughfs_chown(
    void * user_data,
    char const * path,
    uid_t uid,
    gid_t gid)
{
    char * effective_path = passthroughfs_get_effective_path(user_data, path);
    printf("chown: path=%s\n", effective_path);

    int result = lchown(effective_path, uid, gid);
    if (-1 == result)
    {
        result = -errno;
    }

    free(effective_path);
    return result;
}

static int
passthroughfs_truncate(
    void * user_data,
    char const * path,
    off_t size,
    uint64_t file_handle)
{
    char * effective_path = passthroughfs_get_effective_path(user_data, path);
    printf("truncate: path=%s\n", effective_path);

    int result;
    if (FS_INVALID_HANDLE != file_handle)
    {
        result = ftruncate(file_handle, size);
    }
    else
    {
        result = truncate(effective_path, size);
    }

    if (-1 == result)
    {
        result = -errno;
    }

    free(effective_path);
    return result;
}

static int
passthroughfs_create_file(
    void * user_data,
    char const * path,
    mode_t mode,
    int flags,
    uint64_t * file_handle)
{
    char * effective_path = passthroughfs_get_effective_path(user_data, path);
    printf("create: path=%s\n", effective_path);

    int result = open(effective_path, flags, mode);
    if (-1 != result)
    {
        *file_handle = result;
        result = 0;
    }
    else
    {
        result = -errno;
    }

    free(effective_path);
    return result;
}

static int
passthroughfs_open(
    void * user_data,
    char const * path,
    int flags,
    uint64_t * file_handle)
{
    char * effective_path = passthroughfs_get_effective_path(user_data, path);
    printf("open: path=%s\n", effective_path);

    int result = open(effective_path, flags);
    if (-1 != result)
    {
        *file_handle = result;
        result = 0;
    }
    else
    {
        result = -errno;
    }

    free(effective_path);
    return result;    
}

static int
passthroughfs_read(
    void * user_data,
    char const * path,
    char * buffer,
    size_t buffer_size,
    off_t offset,
    uint64_t file_handle)
{
    int result;

    if (FS_INVALID_HANDLE != file_handle)
    {
        printf("read: fd=%ld\n", file_handle);
        result = pread(file_handle, buffer, buffer_size, offset);
        if (-1 == result)
        {
            result = -errno;
        }
    }
    else
    {
        char * effective_path = passthroughfs_get_effective_path(user_data, path);
        printf("read: path=%s\n", effective_path);
        int fd = open(effective_path, O_RDONLY);
        if (0 <= fd)
        {
            result = pread(fd, buffer, buffer_size, offset);
            if (-1 == result)
            {
                result = -errno;
            }
            close(fd);
        }
        else
        {
            result = -errno;
        }
        free(effective_path);
    }

    return result;
}

static int
passthroughfs_write(
    void * user_data,
    char const * path,
    char const * buffer,
    size_t buffer_size,
    off_t offset,
    uint64_t file_handle)
{
    int result;

    if (FS_INVALID_HANDLE != file_handle)
    {
        printf("write: fd=%ld\n", file_handle);
        result = pwrite(file_handle, buffer, buffer_size, offset);
        if (-1 == result)
        {
            result = -errno;
        }
    }
    else
    {
        char * effective_path = passthroughfs_get_effective_path(user_data, path);
        printf("write: path=%s\n", effective_path);
        int fd = open(effective_path, O_RDONLY);
        if (0 <= fd)
        {
            result = pwrite(fd, buffer, buffer_size, offset);
            if (-1 == result)
            {
                result = -errno;
            }
            close(fd);
        }
        else
        {
            result = -errno;
        }
        free(effective_path);
    }

    return result;
}

static int
passthroughfs_statfs(
    void * user_data,
    char const * path,
    struct statvfs * buffer)
{
    char * effective_path = passthroughfs_get_effective_path(user_data, path);
    printf("statfs: path=%s\n", effective_path);

    int result = statvfs(effective_path, buffer);
    if (-1 == result)
    {
        result = -errno;
    }

    return result;
}

static int
passthroughfs_release_handle(
    void * user_data,
    char const * path,
    uint64_t file_handle)
{
    (void) path;
    close(file_handle);
    return 0;
}

static int
passthroughfs_fsync(
    void * user_data,
    char const * path,
    int isdatasync,
    uint64_t file_handle)
{
    (void) path;
    (void) isdatasync;
    (void) file_handle;
    return 0;
}

static off_t
passthroughfs_lseek(
    void * user_data,
    char const * path,
    off_t offset,
    int whence,
    uint64_t file_handle)
{
    off_t result;

    if (FS_INVALID_HANDLE != file_handle)
    {
        printf("lseek: fd=%ld\n", file_handle);
        result = lseek(file_handle, offset, whence);
        if (-1 == result)
        {
            result = -errno;
        }
    }
    else
    {
        char * effective_path = passthroughfs_get_effective_path(user_data, path);
        printf("write: path=%s\n", effective_path);
        int fd = open(effective_path, O_RDONLY);
        if (0 <= fd)
        {
            result = lseek(fd, offset, whence);
            if (-1 == result)
            {
                result = -errno;
            }
            close(fd);
        }
        else
        {
            result = -errno;
        }
        free(effective_path);
    }

    return result;
}



static struct fs_operations const g_passthroughfs_operations =
{
    .init = &passthroughfs_init,
    .cleanup = &passthroughfs_cleanup,
    .getattr = &passthroughfs_getattr,
    .access = &passthroughfs_access,
    .readlink = &passthroughfs_readlink,
    .readdir = &passthroughfs_readdir,
    .mknod = &passthroughfs_mknod,
    .mkdir = &passthroughfs_mkdir,
    .symlink = &passthroughfs_symlink,
    .unlink = &passthroughfs_unlink,
    .rmdir = &passthroughfs_rmdir,
    .rename = &passthroughfs_rename,
    .link = &passthroughfs_link,
    .chmod = &passthroughfs_chmod,
    .chown = &passthroughfs_chown,
    .truncate = &passthroughfs_truncate,
    .open = &passthroughfs_open,
    .create = &passthroughfs_create_file,
    .read = &passthroughfs_read,
    .write = &passthroughfs_write,
    .statfs = &passthroughfs_statfs,
    .release = &passthroughfs_release_handle,
    .fsync = &passthroughfs_fsync,
    .lseek = &passthroughfs_lseek
};


struct passthroughfs *
passthroughfs_create(
    char const * working_dir)
{
    struct passthroughfs * fs = malloc(sizeof(struct passthroughfs));
    fs->working_dir = strdup(working_dir);

    return fs;
}

void
passthroughfs_release(
    struct passthroughfs * fs)
{
    free(fs->working_dir);
    free(fs);
}

struct fs_operations const *
passthroughfs_get_operations(
    struct passthroughfs * fs)
{
    (void) fs;
    return &g_passthroughfs_operations;
}
