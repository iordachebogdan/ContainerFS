#include "fzip.h"

struct fuse_operations FZIP_OPERATIONS = {
    .readdir = fzip_readdir,
};

struct FzipData* get_data() {
    return (struct FzipData*)fuse_get_context()->private_data;
}
