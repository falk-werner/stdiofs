#ifndef RPC_METHODS_H
#define RPC_METHODS_H

#ifdef __cplusplus
extern "C"
{
#endif

#define RPC_METHOD_GETATTR   1
#define RPC_METHOD_ACCESS    2
#define RPC_METHOD_READLINK  3
#define RPC_METHOD_READDIR   4
#define RPC_METHOD_MKNOD     5
#define RPC_METHOD_MKDIR     6
#define RPC_METHOD_SYMLINK   7
#define RPC_METHOD_UNLINK    8
#define RPC_METHOD_RMDIR     9
#define RPC_METHOD_RENAME   10
#define RPC_METHOD_LINK     11
#define RPC_METHOD_CHMOD    12
#define RPC_METHOD_CHOWN    13
#define RPC_METHOD_TRUNCATE 14
#define RPC_METHOD_CREATE   15
#define RPC_METHOD_OPEN     16
#define RPC_METHOD_READ     18
#define RPC_METHOD_WRITE    19
#define RPC_METHOD_STATFS   20
#define RPC_METHOD_RELEASE  21
#define RPC_METHOD_FSYNC    22
#define RPC_METHOD_LSEEK    23

#ifdef __cplusplus
}
#endif

#endif
