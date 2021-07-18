#include "stdiofs/fs/stub_intern.h"
#include "stdiofs/fs/stub_operations.h"
#include "stdiofs/rpc/stub.h"
#include "stdiofs/rpc/deserializer.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

static int
fs_stub_on_methodcall(
    void * user_data,
    struct rpc_buffer *buffer)
{
    puts("methodcall");
    struct fs_stub * stub = user_data;

    int const method_id = rpc_get_method_id(buffer);

    fs_stub_operation_fn * operation = fs_stub_get_operation(method_id);
    int const result = operation(stub, buffer);

    return result;
}

struct fs_stub *
fs_stub_create(
    struct rpc_connection * connection,
    struct fs_operations const * operations,
    void * user_data)
{
    struct fs_stub * stub = malloc(sizeof(struct fs_stub));
    stub->rpc = rpc_stub_create(&fs_stub_on_methodcall, stub, connection);
    stub->operations.init = operations->init;
    stub->operations.cleanup = operations->cleanup;
    stub->operations.getattr = operations->getattr;
    stub->operations.access = operations->access;
    stub->operations.readlink = operations->readlink;
    stub->operations.readdir = operations->readdir;
    stub->operations.mknod = operations->mknod;
    stub->operations.mkdir = operations->mkdir;
    stub->operations.symlink = operations->symlink;
    stub->operations.unlink = operations->unlink;
    stub->operations.rmdir = operations->rmdir;
    stub->operations.rename = operations->rename;
    stub->operations.link = operations->link;
    stub->operations.chown = operations->chown;
    stub->operations.chmod = operations->chmod;
    stub->operations.truncate = operations->truncate;
    stub->operations.create = operations->create;
    stub->operations.open = operations->open;
    stub->operations.read = operations->read;
    stub->operations.write = operations->write;
    stub->operations.statfs = operations->statfs;
    stub->operations.release = operations->release;
    stub->operations.fsync = operations->fsync;
    stub->operations.lseek = operations->lseek;

    stub->user_data = stub->operations.init(user_data);

    return stub;
}

void
fs_stub_release(
    struct fs_stub * stub)
{
    stub->operations.cleanup(stub->user_data);
    rpc_stub_release(stub->rpc);
}

int
fs_stub_service(
    struct fs_stub * stub)
{
    return rpc_stub_service(stub->rpc);
}
