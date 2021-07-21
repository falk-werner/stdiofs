#ifndef RPC_TYPES_H
#define RPC_TYPES_H

#include "stdiofs/rpc/type/stringlist.h"

#ifndef __cplusplus
#include <stddef.h>
#include <inttypes.h>
#else
#include <cstddef>
#include <cinttypes>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define RPC_NONE   0

#define RPC_INT    1
#define RPC_SIZE   2
#define RPC_UID    3
#define RPC_GID    4
#define RPC_MODE   5

#define RPC_UINT64 6
#define RPC_OFFSET 7
#define RPC_DEV    8

#define RPC_STRING  9
#define RPC_BYTES  10

#define RPC_STAT       11
#define RPC_STATVFS    12
#define RPC_STRINGLIST 13


struct rpc_buffer;
struct rpc_arg;

typedef int
rpc_serialize_fn(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

typedef int
rpc_deserialize_fn(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern rpc_serialize_fn *
rpc_get_serializer(int type_id);

extern rpc_deserialize_fn *
rpc_get_deserializer(int type_id);

extern int
rpc_serialize_string(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern void
rpc_serialize_raw_string(
    struct rpc_buffer * buffer,
    char const * value);

extern int
rpc_deserialize_string(
    struct rpc_buffer * buffer,
    struct rpc_arg const *arg);

extern int
rpc_deserialize_raw_string(
    struct rpc_buffer * buffer,
    char * * value);

extern void
rpc_serialize_raw_int(
    struct rpc_buffer * buffer,
    int value);

extern int
rpc_serialize_int(
    struct rpc_buffer * buffer,
    struct rpc_arg const *arg);

extern int
rpc_deserialize_raw_int(
    struct rpc_buffer * buffer,
    int * value);

extern int
rpc_deserialize_int(
    struct rpc_buffer * buffer,
    struct rpc_arg const *arg);


extern int
rpc_serialize_u64(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern void
rpc_serialize_raw_u64(
    struct rpc_buffer * buffer,
    uint64_t value);

extern int
rpc_deserialize_u64(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern int
rpc_deserialize_raw_u64(
    struct rpc_buffer * buffer,
    uint64_t * value);

extern int
rpc_serialize_stat(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern int
rpc_deserialize_stat(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern int
rpc_serialize_offset(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern int
rpc_deserialize_offset(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern int
rpc_serialize_mode(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern int
rpc_deserialize_mode(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern int
rpc_serialize_uid(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern int
rpc_deserialize_uid(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern int
rpc_serialize_gid(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern int
rpc_deserialize_gid(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern int
rpc_serialize_dev(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern int
rpc_deserialize_dev(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern void
rpc_serialize_raw_u32(
    struct rpc_buffer * buffer,
    uint32_t value);

extern int
rpc_serialize_size(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern int
rpc_deserialize_raw_u32(
    struct rpc_buffer * buffer,
    uint32_t * value);

extern int
rpc_deserialize_size(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);


extern int
rpc_serialize_bytes(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern int
rpc_deserialize_bytes(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern int
rpc_serialize_statvfs(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);

extern int
rpc_deserialize_statvfs(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg);


#ifdef __cplusplus
}
#endif

#endif
