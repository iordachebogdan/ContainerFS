#include "data.h"

#include <stdlib.h>

int create(const char* path, struct FzipData** data_) {
    struct FzipData* data = *data_;
    if (!(data = malloc(sizeof(struct FzipData)))
            || create_tree(path, &data->tree)) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int destroy(struct FzipData** data_) {
    struct FzipData* data = *data_;
    if (destroy_tree(&data->tree)) {
        return EXIT_FAILURE;
    }
    free(data);
    return EXIT_SUCCESS;
}
