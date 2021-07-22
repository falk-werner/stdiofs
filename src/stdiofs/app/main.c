#define FUSE_USE_VERSION 31
#define _GNU_SOURCE

#include "stdiofs/proxyfs.h"
#include "stdiofs/fs/proxy.h"
#include "stdiofs/rpc/rpc.h"
#include "stdiofs/rpc/connection.h"

#include <fuse.h>

#include <pthread.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static void
check_args(int argc, char * argv[])
{
    bool show_help = false;
    int result = EXIT_SUCCESS;

    if (argc <= 1)
    {
        fprintf(stderr, "error: missing mountpoint\n");
        show_help = true;
        result = EXIT_SUCCESS;
    }

    for(size_t i = 1; i < argc; i++)
    {
        char const * arg = argv[i];
        if ((0 == strcmp(arg, "-h")) || (0 == strcmp(arg, "--help")))
        {
            show_help = true;
            break;
        }
    }

    if (show_help)
    {
        struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
        fuse_opt_parse(&args, NULL, NULL, NULL);
        fuse_opt_add_arg(&args, "--help");        
        fuse_main(args.argc, args.argv, NULL, NULL);
        fuse_opt_free_args(&args);
        exit(result);
    }
}

int main(int argc, char * argv[])
{
    check_args(argc, argv);

    struct rpc_connection proxy_connection;
    rpc_connection_init(&proxy_connection, STDIN_FILENO, STDOUT_FILENO, 0);

    struct rpc * rpc = rpc_create(&proxy_connection);
    struct fs_proxy * proxy = fs_proxy_create(rpc);
    struct proxyfs * proxyfs = proxyfs_create(proxy);

    umask(0);
    int result = fuse_main(argc, argv, proxyfs_get_operations(proxyfs), proxyfs);

    proxyfs_release(proxyfs);
    fs_proxy_release(proxy);
    rpc_release(rpc);

    return result;
}