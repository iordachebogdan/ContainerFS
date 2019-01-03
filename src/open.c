#include <errno.h>
#include "fzip.h"

int fzip_open(const char *path, struct fuse_file_info *fi) {
    printf("Opening: %s\n", path);
    return -ENOENT;
}
