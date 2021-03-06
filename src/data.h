#ifndef _DATA_H_
#define _DATA_H_

#include <zip.h>
#include "dir_tree.h"
#include <pthread.h>

struct FzipData {
    struct DirTree* tree;
    zip_t* archive;
    pthread_mutex_t lock;
};

int create(const char*, struct FzipData**);
int destroy(struct FzipData*);

#endif
