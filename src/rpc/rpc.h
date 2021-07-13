#ifndef RPC_H
#define RPC_H

#ifdef __cplusplus
extern "C"
{
#endif

struct rpc;
struct rpc_arg;

extern struct rpc *
rpc_create(void);

extern void
rpc_release(
    struct rpc * rpc);

extern int
rpc_invoke(
    struct rpc * rpc,
    int method_id,
    struct rpc_arg const * args);



#ifdef __cplusplus
}
#endif


#endif