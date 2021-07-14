#define FUSE_USE_VERSION 31
#define _GNU_SOURCE

#include "stdiofs/passthroughfs.h"
#include "stdiofs/proxyfs.h"
#include "stdiofs/fs/stub.h"
#include "stdiofs/fs/proxy.h"
#include "stdiofs/rpc/rpc.h"

#include <fuse.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char * argv[])
{
    char * working_dir = get_current_dir_name();
    struct passthroughfs * fs = passthroughfs_create(working_dir);
    struct fs_stub * stub = fs_stub_create(passthroughfs_get_operations(fs), fs);

    struct rpc * rpc = rpc_create();
    struct fs_proxy * proxy = fs_proxy_create(rpc);
    struct proxyfs * proxyfs = proxyfs_create(proxy);

    umask(0);
    int result = fuse_main(argc, argv, proxyfs_get_operations(proxyfs), proxyfs);

    proxyfs_release(proxyfs);
    fs_proxy_release(proxy);
    rpc_release(rpc);
    fs_stub_release(stub);
    passthroughfs_release(fs);
    free(working_dir);

    return result;
}