#ifndef _FZIP_H_
#define _FZIP_H_

#define FUSE_USE_VERSION 31
#include <fuse.h>
#include <stddef.h>
#include <string.h>
#include <zip.h>

#include "data.h"

#define HELP "USAGE\n\
\t./zipfs [OPTION] MOUNTPOINT\n\
\rOPTIONS\n\
\t--filename=FILENAME (specifies archive location, by default this is set to \'./archive.zip\' \n\
\t-f (run in the foreground) \n\
\t-s (run application on a single thread)\n"

struct Options {
    const char* filename;
};

struct FileHandle {
  zip_file_t* file;
  struct DirTree* node;
};

#define OPTION(t, p)                           \
    { t, offsetof(struct Options, p), 1 }
const struct fuse_opt option_spec[2];

extern struct fuse_operations FZIP_OPERATIONS;

struct FzipData* get_data();
int get_zip_error(zip_t*);

void* fzip_init(struct fuse_conn_info*,
                struct fuse_config*);
int fzip_getattr(const char*, struct stat*,
                 struct fuse_file_info*);
int fzip_readdir(const char*, void*, fuse_fill_dir_t, off_t,
                 struct fuse_file_info*, enum fuse_readdir_flags);
int fzip_open(const char*, struct fuse_file_info*);
int fzip_read(const char*, char*, size_t,
        off_t, struct fuse_file_info*);
int fzip_readlink(const char*, char*, size_t);
int fzip_release(const char*, struct fuse_file_info*);

#endif
