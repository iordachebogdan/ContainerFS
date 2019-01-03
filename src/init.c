#include "fzip.h"

void* fzip_init(struct fuse_conn_info* conn,
                struct fuse_config* config) {
    (void) conn;
    config->kernel_cache = 1;
    return (void*) get_data();
}
