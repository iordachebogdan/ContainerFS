#include "data.h"

#include <stdlib.h>

int create(const char* path, struct FzipData** data_) {
    struct FzipData* data = *data_;
    if (!(data = malloc(sizeof(struct FzipData)))
            || create_tree(path, &data->tree)) {
        return EXIT_FAILURE;
    }
    int result = 0;
    data->archive = zip_open(path, ZIP_RDONLY, &result);
    if (result != 0) {
        if (result == ZIP_ER_NOENT) {
            printf("The zip file doesn't exists\n");
        } else if (result == ZIP_ER_EXISTS){
            printf("Exists\n");
        } else if (result == ZIP_ER_INCONS) {
            printf("Inconsistences\n");
        } else {
            printf("Unknown error: %s\n", path);
            printf("%d\n", result);
        }
        return EXIT_FAILURE;
    } else {
        printf("Opened archive: %s\n", path);
    }

    int err = pthread_mutex_init(&(data->lock), NULL);
    if (err) {
        printf("Cannot init mutex\n");
        return err;
    }

    *data_ = data;
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
    int err = pthread_mutex_destroy(&(data->lock));
    if (err)
        return err;
    free(data);
    return EXIT_SUCCESS;
}
