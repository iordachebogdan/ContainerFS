#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <zip.h>

#include "utility.h"
#include "data.h"
#include "fzip.h"

int main(int argc, char** argv) {
    // Objects that need to be destoyed manually
    struct FzipData* data = NULL;
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

    struct Options options;
    options.filename = strdup("archive.zip");
    if (fuse_opt_parse(&args, &options, option_spec, NULL) == -1) {
        return EXIT_FAILURE;
    }

    int result;
    if ((result = create(options.filename, &data))) {
        goto exit;
    }

    result = fuse_main(args.argc, args.argv, &FZIP_OPERATIONS, data);
    fuse_opt_free_args(&args);

exit:
    destroy(data);

    if (result) {
        perror("");
    }
    return result;
}
