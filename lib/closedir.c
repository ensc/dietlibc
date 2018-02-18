#include "dietdirent.h"
#include <sys/mman.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>

#include "../dietpagesize.h"

int closedir (DIR* d) {
  int res=close(d->fd);
#ifdef __DIR_OFFSET_COOKIE
  free(d->offsets);
#endif
  munmap (d, __DIET_PAGE_SIZE);
  return res;
}
