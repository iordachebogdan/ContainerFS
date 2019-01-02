#ifndef _DATA_H_
#define _DATA_H_

#include <zip.h>
#include "dir_tree.h"
#include "fzip.h"

struct FzipData {
    struct DirTree* tree;
    zip_t* archive;
};

int create(const char*, struct FzipData**, struct Options*);
int destroy(struct FzipData*);

#endif
