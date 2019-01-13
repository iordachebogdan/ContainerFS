#include <errno.h>
#include "fzip.h"
#include <stdlib.h>

int fzip_open(const char *path, struct fuse_file_info *fi) {
    printf("[open] Opening: %s\n", path);
    zip_t* archive = get_data()->archive;
    if (*path == '\0') {
        return -ENOENT;
    }

    //locate file
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
    printf("[open] File was found\n");

    //open file
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
    printf("[open] File was opened\n");

    struct FileHandle* fh = (struct FileHandle*)malloc(sizeof(struct FileHandle));
    fh->file = file;
    zip_stat_t stat;
    if (zip_stat_index(archive, index, 0, &stat) != 0) {
        free(fh);
        int ze = get_zip_error(archive);
        zip_error_clear(archive);
        switch (ze) {
            case ZIP_ER_MEMORY: return -ENOMEM;
            case ZIP_ER_INVAL: return -EINVAL;
            case ZIP_ER_NOENT: return -ENOENT;
            default: exit(EXIT_FAILURE);
        }
    }
    if (!(stat.valid & ZIP_STAT_SIZE)) {
        free(fh);
        printf("[open] Invalid size\n");
        return -EINVAL;
    }

    struct DirTree* node = find(get_data()->tree, path + 1);
    if (node == NULL) {
        free(fh);
        printf("[open] Cannot find DirTree node\n");
        return -ENOENT;
    }
    fh->node = node;

    pthread_mutex_lock(&(get_data()->lock));
    node->open_count += 1;
    if (node->open_count == 1) {
        pthread_mutex_unlock(&(get_data()->lock));
        node->file_data = (char*)calloc(stat.size, sizeof(char));
        if (node->file_data != NULL) {
            node->file_data_size = stat.size;
            printf("[open] Successfully allocated %ld bytes of data\n", stat.size);

            char* pos_in_buff = node->file_data;
            int remaining = stat.size;
            while (remaining > 0) {
                zip_int64_t read_bytes = zip_fread(file, pos_in_buff, remaining);
                if (read_bytes == 0) {
                    //reached EOF prematurely
                    break;
                } else if (read_bytes == -1) {
                    //error while reading
                    free(node->file_data);
                    node->open_count = node->file_data_size = 0;
                    free(fh);
                    return -EIO;
                }
                remaining -= read_bytes;
                pos_in_buff += read_bytes;
            }
        } else {
            printf("[open] Not enough space for file allocation\n");
        }
    } else {
        pthread_mutex_unlock(&(get_data()->lock));
    }

    //set file handle
    fi->fh = (uint64_t)fh;

    return 0;
}
