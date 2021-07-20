#include <sys/statvfs.h>

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


int main(int argc, char *argv[])
{
    int result = EXIT_FAILURE;

    char const * path = (argc > 1) ? argv[1] : ".";
    struct statvfs buffer;
    int rc = statvfs(path, &buffer);

    if (0 == rc)
    {
        printf("path: %s\n", path);
        printf(" f_bsize  : block size           : %ld\n", buffer.f_bsize);
        printf(" f_frsize : fragment size        : %ld\n", buffer.f_frsize);
        printf(" f_blocks : total data blocks    : %" PRIu64 "\n", buffer.f_blocks);
        printf(" f_bfree  : free blocks          : %" PRIu64 "\n", buffer.f_bfree);
        printf(" f_bavail : free blocks available: %" PRIu64 "\n", buffer.f_bavail);
        printf(" f_files  : total inodes         : %" PRIu64 "\n", buffer.f_files);
        printf(" f_ffree  : free inodes          : %" PRIu64 "\n", buffer.f_ffree);
        printf(" f_favail : free inodes for users: %" PRIu64 "\n", buffer.f_favail);
        printf(" f_fsid   : filesystem ID        : %ld\n", buffer.f_fsid);
        printf(" f_flag   : mount flags          : %lu\n", buffer.f_flag);
        printf(" f_namemax: max. filename length : %lu\n", buffer.f_namemax);
        result = EXIT_SUCCESS;
    }
    else
    {
        perror("statfs");
    }

    return result;
}