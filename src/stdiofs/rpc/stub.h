#ifndef RPC_STUB_H
#define RPC_STUB_H

#ifdef __cplusplus
extern "C"
{
#endif

struct rpc_stub;
struct rpc_buffer;
struct rpc_connection;

typedef int
rpc_stub_callback_fn(
    void * user_data,
    struct rpc_buffer * buffer);

extern struct rpc_stub *
rpc_stub_create(
    rpc_stub_callback_fn * callback,
    void * user_data,
    struct rpc_connection * connection);

extern void
rpc_stub_release(
    struct rpc_stub * stub);

extern int
rpc_stub_service(
    struct rpc_stub * stub);

#ifdef __cplusplus
}
#endif

#endif
