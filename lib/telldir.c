#include "dietdirent.h"
#include <unistd.h>
#include <dirent.h>

long telldir(DIR *d) {
  long result = 0;
  if (lseek(d->fd,0,SEEK_CUR)) {
    if (d->is_64) {
      struct linux_dirent64 *ld = (void *)(d->buf + d->cur);
      result=ld->d_off;
    } else {
      struct linux_dirent *ld = (void *)(d->buf + d->cur);
      result=ld->d_off;
    }
  }
  return result;
}
