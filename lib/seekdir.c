#include "dietdirent.h"
#include <unistd.h>
#include <dirent.h>

void seekdir(DIR *d,long offset) {
  if (lseek(d->fd,offset,SEEK_SET) != (off_t)-1) {
    d->num=d->cur=0;
    if (d->is_64) {
      struct linux_dirent64 *ld = (void *)(d->buf + d->cur);
      ld->d_off = offset;
    } else {
      struct linux_dirent *ld = (void *)(d->buf + d->cur);
      ld->d_off = offset;
    }
  }
}
