#include "dietdirent.h"
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>

int closedir (DIR* d) {
  int res=close(d->fd);
  free(d);
  return res;
}
