#ifndef RPC_SERIALIZER_H
#define RPC_SERIALIZER_H

#ifdef __cplusplus
extern "C"
{
#endif

struct rpc_arg;
struct rpc_buffer;

/// \brief Serialized a message into a buffer.
///
/// \param buffer    buffer to serialize to
/// \param direction either RPC_IN (request) or RPC_OUT (response)
/// \param method_id id of the method to serialize
/// \param args      args to serialize
///
/// \return 0 on success, otherwise failure
extern int
rpc_serialize(
    struct rpc_buffer * buffer,
    int direction,
    int method_id,
    struct rpc_arg const * args);

#ifdef __cplusplus
}
#endif

#endif
