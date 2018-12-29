#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#include "utility.h"
#include "data.h"
#include "fzip.h"

int main(int argc, char** argv) {
    // Objects that need to be destoyed manually
    struct FzipData* data = NULL;
    char** fuse_argv = NULL;

    int result;
    if (argc < 2) {
        errno = EINVAL;
        result = EXIT_FAILURE;
        goto exit;
    }

    if ((result = create(argv[1], &data))) {
        goto exit;
    }

    // discard argv[1]
    fuse_argv = malloc(argc - 1);
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
    destroy(data);
    FREE(fuse_argv);

    if (result) {
        perror("");
    }
    return result;
}
