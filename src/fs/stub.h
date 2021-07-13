#ifndef FS_STUB_H
#define FS_STUB_H

#ifdef __cplusplus
extern "C"
{
#endif

struct fs_stub;
struct fs_operations;

extern struct fs_stub *
fs_stub_create(
    struct fs_operations const * operations,
    void * user_data);

extern void
fs_stub_release(
    struct fs_stub * stub);

extern void
fs_stub_service(
    struct fs_stub * stub);

#ifdef __cplusplus
}
#endif

#endif
