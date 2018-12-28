#include "fzip.h"

#include <stdio.h>

int fzip_readdir(const char* path, void* buf, fuse_fill_dir_t filler,
                 off_t offset, struct fuse_file_info* fi,
                 enum fuse_readdir_flags flags) {
    (void)offset; (void)fi; (void)flags;
    printf("readdir: %s, offset = %lu\n", path, offset);
    return 0;
}