#include "fzip.h"
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int fzip_getattr(const char* path, struct stat* stbuf,
                 struct fuse_file_info* fi) {
    (void) fi;
    printf("Getting attributes: %s\n", path);
    zip_t* archive = get_data()->archive;
    memset(stbuf, 0, sizeof(struct stat));
    if (strcmp(path, "/") == 0) {
        // Read and execute permissions for directories
		stbuf->st_mode = S_IFDIR | 0555;
		stbuf->st_nlink = 2;
        return 0;
	}

    zip_stat_t stat;
    int found = 0;
    // Search as a file
    if (zip_stat(archive, path + 1, 0, &stat) != 0) {
        int ze = get_zip_error(archive);
        zip_error_clear(archive);
        switch (ze) {
            case ZIP_ER_MEMORY: return -ENOMEM;
            case ZIP_ER_INVAL: return -EINVAL;
            case ZIP_ER_NOENT: break;
            default: exit(EXIT_FAILURE);
        }
    } else {
        printf("Found the first time\n");
    }
    // Search as a directory - with '/' at the end of 'path'
    if (!found) {
        printf("Not found the first time\n");
        int len = strlen(path);
        char* dir_path = malloc(len + 2);
        strcpy(dir_path, path);
        dir_path[len] = '/';
        dir_path[len + 1] = '\0';
        int res = zip_stat(archive, dir_path + 1, 0, &stat);
        free(dir_path);
        if (res != 0) {
            int ze = get_zip_error(archive);
            zip_error_clear(archive);
            switch (ze) {
                case ZIP_ER_MEMORY: return -ENOMEM;
                case ZIP_ER_INVAL: return -EINVAL;
                case ZIP_ER_NOENT: return -ENOENT;
                default: exit(EXIT_FAILURE);
            }
        }    
    }
    zip_uint8_t opsys;
    zip_uint32_t attributes;
    // The following call shouldn't fail
    zip_file_get_external_attributes(archive, stat.index, 0,
                &opsys, &attributes);
    if (opsys != ZIP_OPSYS_UNIX) {
        return -ENOTSUP;
    }
    printf("%d\n", attributes);
    mode_t mode = (mode_t) ((attributes >> 16) & 0xFFFF);
    printf("%d\n", mode);
    if ((mode & S_IFMT) == S_IFDIR) {
        // Read and execute permissions for directories
        stbuf->st_mode = S_IFDIR | 0555;
        stbuf->st_nlink = 2;
        printf("Directory\n");
    } else {
        // Read permissions mainly for other kind of files
        stbuf->st_mode = (mode & S_IFMT) | 0444;
        // If the file has an execute attribute on it in the
        // archive, it will also have it on the filesystem
        stbuf->st_mode |= (mode & S_IXUSR) |
            (mode & S_IXGRP) |
            (mode & S_IXOTH);
        stbuf->st_nlink = 1;
    }
    if ((mode & S_IFMT) == S_IFREG) {
        stbuf->st_size = stat.size;
        printf("Reg file\n");
    }
    printf("Getattr succes\n");
    printf("Mode: %d\n", stbuf->st_mode);
    return 0;
}
