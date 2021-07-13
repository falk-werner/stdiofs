#ifndef PASSTHROUGHFS_H
#define PASSTHROUGHFS_H

#ifdef __cplusplus
extern "C"
{
#endif

struct fs_operations;
struct passthroughfs;

extern struct passthroughfs *
passthroughfs_create(
    char const * working_dir);

extern void
passthroughfs_release(
    struct passthroughfs * fs);

extern struct fs_operations const *
passthroughfs_get_operations(
    struct passthroughfs * fs);

#ifdef __cplusplus
}
#endif

#endif
