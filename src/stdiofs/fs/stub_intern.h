#ifndef FS_STUB_INTERN_H
#define FS_STUB_INTERN_H

#include "stdiofs/fs/stub.h"
#include "stdiofs/fs/operations.h"

#ifdef __cplusplus
extern "C"
{    
#endif

struct fs_stub
{
    struct fs_operations operations;
    void * user_data;
};

#ifdef __cplusplus
}
#endif

#endif
