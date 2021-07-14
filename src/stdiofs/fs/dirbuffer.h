#ifndef FS_DIRBUFFER_H
#define FS_DIRBUFFER_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef int fs_dirbuffer_add_fn(
    void * buffer,
    char const * name);


#ifdef __cplusplus
}
#endif

#endif