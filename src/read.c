#include "fzip.h"
#include "utility.h"
#include <errno.h>
#define MAX_READ_BUFF 1024

int fzip_read(const char* path, char* buff, size_t size,
        off_t offset, struct fuse_file_info* fi) {
    printf("Reading: %s\n", path);

    //skip first offset bytes
    zip_file_t* file = (zip_file_t*)(fi->fh);
    if (zip_fseek(file, (zip_int64_t)offset, SEEK_SET)) {
        //error while seeking
        return -ENOTSUP;
    }

    char* pos_in_buff = buff;
    size_t remaining = size;
    while (remaining > 0) {
        zip_int64_t read_bytes = zip_fread(file, pos_in_buff, remaining);
        if (read_bytes == 0) {
            //reached EOF prematurely
            return size - remaining;
        } else if (read_bytes == -1) {
            //error while reading
            return -EIO;
        }
        remaining -= read_bytes;
        pos_in_buff += read_bytes;
    }

    return size;
}
