#ifndef RPC_TYPE_STRINGLIST_H
#define RPC_TYPE_STRINGLIST_H

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
rpc_stringlist_add_fn(
    void * buffer,
    char const * value);

struct rpc_stringlist
{
    char * * entries;
    size_t size;
    size_t capacity;
};

extern void
rpc_stringlist_init(
    struct rpc_stringlist * buffer);

extern void
rpc_stringlist_cleanup(
    struct rpc_stringlist * buffer);

extern int
rpc_stringlist_add(
    void * buffer,
    char const * name);

extern int
rpc_serialize_stringlist(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern int
rpc_deserialize_stringlist(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);


#ifdef __cplusplus
}
#endif


#endif
