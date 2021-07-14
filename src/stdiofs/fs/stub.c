#include "stdiofs/fs/stub_intern.h"

#include <stdlib.h>
#include <string.h>

struct fs_stub *
fs_stub_create(
    struct fs_operations const * operations,
    void * user_data)
{
    struct fs_stub * stub = malloc(sizeof(struct fs_stub));
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
}

void
fs_stub_service(
    struct fs_stub * stub)
{
    // ToDo: implement me
    (void) stub;
}
