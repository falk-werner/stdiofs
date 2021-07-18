#define FUSE_USE_VERSION 31
#define _GNU_SOURCE

#include "stdiofs/passthroughfs.h"
#include "stdiofs/proxyfs.h"
#include "stdiofs/fs/stub.h"
#include "stdiofs/fs/proxy.h"
#include "stdiofs/rpc/rpc.h"
#include "stdiofs/rpc/connection.h"

#include <fuse.h>

#include <pthread.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

static void *
stub_run(void * user_data)
{
    struct rpc_connection * connection = user_data;

    char * working_dir = get_current_dir_name();
    struct passthroughfs * fs = passthroughfs_create(working_dir);
    struct fs_stub * stub = fs_stub_create(connection, passthroughfs_get_operations(fs), fs);

    while (0 == fs_stub_service(stub))
    {
        // loop
    }

    puts("stop stub");
    fs_stub_release(stub);
    passthroughfs_release(fs);
    free(working_dir);

    return NULL;
}

int main(int argc, char * argv[])
{
    int stub_read, stub_write, proxy_read, proxy_write;

    int fds[2];
    int rc = pipe(fds);
    if (0 != rc)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    stub_read = fds[0];
    proxy_write = fds[1];

    fds[2];
    rc = pipe(fds);
    if (0 != rc)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    proxy_read = fds[0];
    stub_write = fds[1];


    struct rpc_connection stub_connection;
    rpc_connection_init(&stub_connection, stub_read, stub_write, 1);
    pthread_t stub_thread;
    rc = pthread_create(&stub_thread, NULL, &stub_run, &stub_connection);
    if (0 != rc)
    {
        perror("pthread_create");
        exit(EXIT_SUCCESS);
    }

    struct rpc_connection proxy_connection;
    rpc_connection_init(&proxy_connection, proxy_read, proxy_write, 1);

    struct rpc * rpc = rpc_create(&proxy_connection);
    struct fs_proxy * proxy = fs_proxy_create(rpc);
    struct proxyfs * proxyfs = proxyfs_create(proxy);

    umask(0);
    int result = fuse_main(argc, argv, proxyfs_get_operations(proxyfs), proxyfs);

    proxyfs_release(proxyfs);
    fs_proxy_release(proxy);
    rpc_release(rpc);

    pthread_join(stub_thread, NULL);
    puts("finished");
    return result;
}