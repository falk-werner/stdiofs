#ifndef RPC_TYPE_DIRBUFFER_H
#define RPC_TYPE_DIRBUFFER_H

#ifndef __cplusplus
#include <stddef.h>
#else
#include <cstddef>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct rpc_buffer;
struct rpc_arg;

typedef int
rpc_dirbuffer_add_fn(
    void * buffer,
    char const * name);

struct rpc_dirbuffer
{
    char * * entries;
    size_t size;
    size_t capacity;
};

extern void
rpc_dirbuffer_init(
    struct rpc_dirbuffer * buffer);

extern void
rpc_dirbuffer_cleanup(
    struct rpc_dirbuffer * buffer);

extern int
rpc_dirbuffer_add(
    void * buffer,
    char const * name);

extern int
rpc_serialize_dirbuffer(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern int
rpc_deserialize_dirbuffer(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);


#ifdef __cplusplus
}
#endif


#endif
