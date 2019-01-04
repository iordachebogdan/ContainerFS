#include "fzip.h"

const struct fuse_opt option_spec[] = {
	OPTION("--filename=%s", filename),
	FUSE_OPT_END
};

struct fuse_operations FZIP_OPERATIONS = {
    .init = fzip_init,
    .readdir = fzip_readdir,
    .getattr = fzip_getattr,
    .open = fzip_open,
    .read = fzip_read,
    .readlink = fzip_readlink,
};

struct FzipData* get_data() {
    return (struct FzipData*)fuse_get_context()->private_data;
}

int get_zip_error(zip_t* archive) {
    zip_error_t* ze = zip_get_error(archive);
    if (ze == NULL) {
        return 0;
    }
    return zip_error_code_zip(ze);
}
