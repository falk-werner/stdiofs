#ifndef FS_STUB_OPERATIONS_H
#define FS_STUB_OPERATIONS_H

#ifdef __cplusplus
extern "C"
{
#endif

struct rpc_buffer;
struct fs_stub;

typedef int
fs_stub_operation_fn(
    struct fs_stub * stub,
    struct rpc_buffer * buffer);

extern fs_stub_operation_fn *
fs_stub_get_operation(
    int method_id);


#ifdef __cplusplus
}
#endif

#endif
