#ifndef RPC_CONNECTION_H
#define RPC_CONNECTION_H

#ifdef __cplusplus
extern "C"
{
#endif

struct rpc_buffer;

struct rpc_connection
{
    int read_fd;
    int write_fd;
    int close_on_cleanup;
};

extern void
rpc_connection_init(
    struct rpc_connection * connection,
    int read_fd,
    int write_fd,
    int close_on_cleanup);

extern void
rpc_connection_cleanup(
    struct rpc_connection * connection);

extern int
rpc_connection_write(
    struct rpc_connection * connection,
    struct rpc_buffer const * buffer);


extern int
rpc_connection_read(
    struct rpc_connection * connection,
    struct rpc_buffer * buffer);

#ifdef __cplusplus
}
#endif

#endif
