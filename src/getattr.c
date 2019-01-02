#include "fzip.h"
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int fzip_getattr(const char* path, struct stat* stbuf,
                 struct fuse_file_info* fi) {
    (void) fi;
    zip_t* archive = get_data()->archive;
    memset(stbuf, 0, sizeof(stbuf));
    if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
        return EXIT_SUCCESS;
	}

    zip_stat_t stat;
    int res;
    if (res = zip_stat(archive, path + 1, 0, &stat)) {
        return -ENOENT;
    }
    zip_uint8_t opsys;
    zip_uint32_t attributes;
    if (res = zip_file_get_external_attributes(archive, stat.index, 0,
                &opsys, &attributes)) {
        return EXIT_FAILURE;
    }
    if (opsys != ZIP_OPSYS_UNIX) {
        return EXIT_FAILURE;
    }
    stbuf->st_mode = attributes;
    if (attributes & S_IFREG) {
        stbuf->st_size = stat.size;
    }
    if (attributes & S_IFDIR) {
        stbuf->st_nlink = 2;
    } else {
        stbuf->st_nlink = 1;
    }
    return EXIT_SUCCESS;
}
