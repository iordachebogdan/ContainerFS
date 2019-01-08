#include <errno.h>
#include "fzip.h"
#include <stdlib.h>

int fzip_open(const char *path, struct fuse_file_info *fi) {
    printf("Opening: %s\n", path);
    zip_t* archive = get_data()->archive;
    if (*path == '\0') {
        return -ENOENT;
    }

    zip_uint64_t index = zip_name_locate(archive, path + 1, 0);
    if (index == -1) {
        int ze = get_zip_error(archive);
        zip_error_clear(archive);
        switch (ze) {
            case ZIP_ER_INVAL: return -EINVAL;
            case ZIP_ER_MEMORY: return -ENOMEM;
            case ZIP_ER_NOENT: return -ENOENT;
            default: exit(EXIT_FAILURE);
        }
    }
    printf("File was found\n");

    zip_file_t* file = zip_fopen_index(archive, index, 0);
    if (file == NULL) {
        int ze = get_zip_error(archive);
        zip_error_clear(archive);
        switch (ze) {
            case ZIP_ER_COMPNOTSUPP:
            case ZIP_ER_ENCRNOTSUPP: return -ENOTSUP;
            case ZIP_ER_MEMORY: return -ENOMEM;
            case ZIP_ER_READ:
            case ZIP_ER_SEEK: return -EIO;
            case ZIP_ER_NOPASSWD:
            case ZIP_ER_WRONGPASSWD: return -EACCES;
            case ZIP_ER_INVAL: return -EINVAL;
            case ZIP_ER_NOENT: return -ENOENT;
            default: exit(EXIT_FAILURE);
        }
    }
    printf("File was opened\n");

    fi->fh = (uint64_t)file;

    return 0;
}
