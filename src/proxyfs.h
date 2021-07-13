#ifndef PROXYFS_H
#define PROXYFS_H

#ifdef __cplusplus
extern "C"
{
#endif

struct proxyfs;
struct fs_proxy;
struct fuse_operations;

extern struct proxyfs *
proxyfs_create(
    struct fs_proxy * proxy);

extern void
proxyfs_release(
    struct proxyfs * fs);

extern struct fuse_operations const *
proxyfs_get_operations(
    struct proxyfs * fs);


#ifdef __cplusplus
}
#endif

#endif
