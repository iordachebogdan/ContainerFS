#include "fzip.h"

const struct fuse_opt option_spec[] = {
	OPTION("--filename=%s", filename),
	FUSE_OPT_END
};

struct fuse_operations FZIP_OPERATIONS = {
    .init = fzip_init,
    .readdir = fzip_readdir,
    .getattr = fzip_getattr,
};

struct FzipData* get_data() {
    return (struct FzipData*)fuse_get_context()->private_data;
}
