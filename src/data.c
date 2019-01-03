#include "data.h"

#include <stdlib.h>

int create(const char* path, struct FzipData** data_) {
    struct FzipData* data = *data_;
    if (!(data = malloc(sizeof(struct FzipData)))
            || create_tree(path, &data->tree)) {
        return EXIT_FAILURE;
    }
    int result;
    data->archive = zip_open(path, ZIP_RDONLY, &result);
    if (result != 0) {
        if (result == ZIP_ER_NOENT) {
            printf("The zip file doesn't exists\n");
        }
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int destroy(struct FzipData* data) {
    if (data == NULL) {
        return EXIT_SUCCESS;
    }

    if (destroy_tree(data->tree)) {
        return EXIT_FAILURE;
    }
    if (zip_close(data->archive)) {
        return EXIT_FAILURE;
    }
    free(data);
    return EXIT_SUCCESS;
}
