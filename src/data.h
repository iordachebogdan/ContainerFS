#ifndef _DATA_H_
#define _DATA_H_

#include "dir_tree.h"

struct FzipData {
    struct DirTree* tree;
};

int create(const char*, struct FzipData**);
int destroy(struct FzipData*);

#endif
