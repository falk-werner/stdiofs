#ifndef RPC_ARG_H
#define RPC_ARG_H

#ifdef __cplusplus
extern "C"
{
#endif

#define RPC_IN  1
#define RPC_OUT 2
#define RPC_END   0


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