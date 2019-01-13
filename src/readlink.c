#include "fzip.h"
#include <stdlib.h>
#include <errno.h>

int fzip_readlink(const char* path, char* buff, size_t len) {
    printf("[readlink] Reading link with buffer of size: %ld\n", len);
    zip_t* archive = get_data()->archive;
    zip_file_t* file = zip_fopen(archive, path + 1, 0);
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
    zip_int64_t read_len = zip_fread(file, buff, len);
    if (read_len < 0) {
        return -EIO;
    }
    if (read_len < len) {
        buff[read_len] = '\0';
    }
    return 0;
}
