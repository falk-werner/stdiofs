#ifndef RPC_ARG_H
#define RPC_ARG_H

#ifdef __cplusplus
extern "C"
{
#endif

#define RPC_IN  1
#define RPC_OUT 2
#define RPC_END   0

#define RPC_NONE   0
#define RPC_STRING 1
#define RPC_INT    2
#define RPC_UINT64 3
#define RPC_STAT   4
#define RPC_OFFSET 5
#define RPC_MODE   6
#define RPC_DIRBUFFER 7
#define RPC_DEV 8
#define RPC_SIZE 9
#define RPC_BYTES 10
#define RPC_STATFS 11

struct rpc_arg
{
    char const * name;
    int direction;
    int type;
    void * value;
    void * extra;
};

#ifdef __cplusplus
}
#endif


#endif