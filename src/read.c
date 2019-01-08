#include "fzip.h"
#include "utility.h"
#include <errno.h>
#define MAX_READ_BUFF 1024

int fzip_read(const char* path, char* buff, size_t size,
        off_t offset, struct fuse_file_info* fi) {
    printf("Reading: %s\n", path);

    //skip first offset bytes
    char* offset_buff[MAX_READ_BUFF];
    zip_file_t* file = (zip_file_t*)(fi->fh);
    while (offset > 0) {
        zip_int64_t bytes_to_read = MIN(offset, MAX_READ_BUFF);
        offset -= bytes_to_read;
        zip_int64_t read_bytes;
        while (bytes_to_read > 0
            && (read_bytes = zip_fread(file, offset_buff, bytes_to_read)) > 0) {
            bytes_to_read -= read_bytes;
        }
        if (bytes_to_read == 0)
            continue;
        memset(buff, 0, size);
        if (read_bytes == 0) {
            //offset too big
            return -EINVAL;
        }
        if (read_bytes < 0) {
            //error while reading
            return -EIO;
        }
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
