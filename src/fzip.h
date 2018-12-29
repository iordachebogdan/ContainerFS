#ifndef _FZIP_H_
#define _FZIP_H_

#define FUSE_USE_VERSION 31
#include <fuse.h>

#include "data.h"

extern struct fuse_operations FZIP_OPERATIONS;

struct FzipData* get_data();

int fzip_readdir(const char*, void*, fuse_fill_dir_t, off_t,
                 struct fuse_file_info*, enum fuse_readdir_flags);

#endif
