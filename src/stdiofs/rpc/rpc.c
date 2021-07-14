#include "stdiofs/rpc/rpc.h"
#include <stdlib.h>

struct rpc
{
    int dummy;
};

struct rpc *
rpc_create(void)
{
    struct rpc * rpc = malloc(sizeof(struct rpc));
    rpc->dummy = 42;
    return rpc;
}

void
rpc_release(
    struct rpc * rpc)
{
    free(rpc);
}


int
rpc_invoke(
    struct rpc * rpc,
    int method_id,
    struct rpc_arg const * args)
{
    (void) rpc;
    (void) method_id;
    (void) args;
    return -1;
}
