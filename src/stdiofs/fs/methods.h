#ifndef FS_METHODS_H
#define FS_METHODS_H

#ifdef __cplusplus
extern "C"
{
#endif

#define FS_METHOD_GETATTR   1
#define FS_METHOD_ACCESS    2
#define FS_METHOD_READLINK  3
#define FS_METHOD_READDIR   4
#define FS_METHOD_MKNOD     5
#define FS_METHOD_MKDIR     6
#define FS_METHOD_SYMLINK   7
#define FS_METHOD_UNLINK    8
#define FS_METHOD_RMDIR     9
#define FS_METHOD_RENAME   10
#define FS_METHOD_LINK     11
#define FS_METHOD_CHMOD    12
#define FS_METHOD_CHOWN    13
#define FS_METHOD_TRUNCATE 14
#define FS_METHOD_CREATE   15
#define FS_METHOD_OPEN     16
#define FS_METHOD_READ     18
#define FS_METHOD_WRITE    19
#define FS_METHOD_STATFS   20
#define FS_METHOD_RELEASE  21
#define FS_METHOD_FSYNC    22
#define FS_METHOD_LSEEK    23

#ifdef __cplusplus
}
#endif

#endif
