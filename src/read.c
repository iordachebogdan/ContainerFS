#include "fzip.h"

int fzip_read(const char* path, char* buff, size_t size,
        off_t offset, struct fuse_file_info* fi) {
	(void) fi;
    (void) path;
    (void) buff;
    (void) offset;
    (void) fi;
    printf("Reading: %s\n", path);
    return 0;
}
