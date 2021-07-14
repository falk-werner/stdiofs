#ifndef RPC_BUFFER_H
#define RPC_BUFFER_H

#ifndef __cplusplus
#include <stddef.h>
#else
#include <cstddef>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct rpc_buffer
{
    char * data;
    size_t size;
    size_t capacity;
};

extern void
rpc_buffer_init(
    struct rpc_buffer * buffer,
    size_t initial_capacity);

extern void
rpc_buffer_cleanup(
    struct rpc_buffer * buffer);

extern void
rpc_buffer_reset(
    struct rpc_buffer * buffer);

extern size_t
rpc_buffer_size(
    struct rpc_buffer * buffer);

extern void
rpc_buffer_write(
    struct rpc_buffer * buffer,
    char const * data,
    size_t count);

extern void
rpc_buffer_write_at(
    struct rpc_buffer * buffer,
    size_t offset,
    char * data,
    size_t count);

extern void
rpc_buffer_skip(
    struct rpc_buffer * buffer,
    size_t count);

extern char *
rpc_buffer_read(
    struct rpc_buffer * buffer,
    char * data,
    size_t count);

#ifdef __cplusplus
}
#endif


#endif
