#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#define FUSE_USE_VERSION 31
#include <fuse.h>
#include <zip.h>

#include "utility.h"
#include "data.h"

#include "readdir.h"

struct fuse_operations FZIP_OPERATIONS = {
    .readdir = fzip_readdir,
};

int main(int argc, char** argv) {
    if (argc < 2) {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    struct fzip_data* data = malloc(sizeof(struct fzip_data));
    int result;
    if (!data || build_tree(argv[1], data)) {
        result = EXIT_FAILURE;
        goto exit;
    }

    // discard argv[1]
    char** fuse_argv = malloc(argc - 1);
    if (!fuse_argv) {
        result = EXIT_FAILURE;
        goto exit;
    }

    fuse_argv[0] = argv[0];
    for (int i = 1; i < argc - 1; ++i) {
        fuse_argv[i] = argv[i + 1];
    }

    result = fuse_main(argc - 1, fuse_argv, &FZIP_OPERATIONS, data);

exit:
    FREE(data, fuse_argv);
    return result;
}
