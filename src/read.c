#include "fzip.h"
#include "utility.h"
#include <errno.h>

int fzip_read(const char* path, char* buff, size_t size,
        off_t offset, struct fuse_file_info* fi) {
    printf("[read] Reading %ld bytes with offset %ld from: %s\n", size, offset, path);

    memset(buff, 0, size);
    struct FileHandle* fh = (struct FileHandle*)(fi->fh);

    if (fh->node->file_data != NULL) {
        printf("[read] File is stored. Read from Memory\n");
        if (offset > fh->node->file_data_size) {
            //offset too big, nothing to read
            return 0;
        }
        if (offset + size > fh->node->file_data_size) {
            //read less than size
            memcpy(buff, fh->node->file_data + offset, fh->node->file_data_size - offset);
            return fh->node->file_data_size - offset;
        } else {
            //read size bytes
            memcpy(buff, fh->node->file_data + offset, size);
            return size;
        }
    }

    printf("[read] File is not stored. Try reading directly\n");
    //skip first offset bytes
    zip_file_t* file = (zip_file_t*)(fi->fh);
    if (offset > 0 && zip_fseek(file, (zip_int64_t)offset, SEEK_SET)) {
        //error while seeking
        printf("[read] zip_fseek not supported\n");
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
