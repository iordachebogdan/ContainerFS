#include "fzip.h"

struct fuse_operations FZIP_OPERATIONS = {
    .readdir = fzip_readdir,
};
