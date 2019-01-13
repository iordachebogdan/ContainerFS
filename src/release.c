#include "fzip.h"
#include <stdlib.h>
#include <errno.h>

int fzip_release(const char* path, struct fuse_file_info* fi) {
  printf("[release] Releasing %s\n", path);
  struct FileHandle* fh = (struct FileHandle*)(fi->fh);

  pthread_mutex_lock(&(get_data()->lock));
  fh->node->open_count -= 1;
  if (fh->node->open_count == 0 && fh->node->file_data != NULL) {
    pthread_mutex_unlock(&(get_data()->lock));
    printf("[release] Last file descriptor for %s is closing\n", path);
    printf("[release] Releasing memory\n");
    free(fh->node->file_data);
    fh->node->file_data = NULL;
    fh->node->file_data_size = 0;
  } else {
    pthread_mutex_unlock(&(get_data()->lock));
  }
  return zip_fclose(fh->file);
}
