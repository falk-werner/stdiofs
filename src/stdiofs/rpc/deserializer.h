#ifndef RPC_DESERIALIZER_H
#define RPC_DESERIALIZER_H

#ifdef __cplusplus
extern "C"
{
#endif

struct rpc_arg;
struct rpc_buffer;

extern int
rpc_get_method_id(
    struct rpc_buffer const * buffer);

extern int
rpc_deserialize(
    struct rpc_buffer const * buffer,
    int direction,
    struct rpc_arg const * args);

#ifdef __cplusplus
}
#endif

#endif
