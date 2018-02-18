#define _FILE_OFFSET_BITS       64

#include "dietdirent.h"
#include <unistd.h>
#include <dirent.h>

#ifdef __DIR_OFFSET_COOKIE
static off64_t cookie_to_offset(DIR *d, long cookie)
{
  if (!d->is_64)
    return cookie;

  if (cookie < 0 || (unsigned long)cookie >= d->num_offsets)
    return (off64_t)-1;

  return d->offsets[cookie].d_off;
}
#else
inline static long cookie_to_offset(DIR *d, long cookie)
{
  return cookie;
}
#endif

void seekdir(DIR *d,long offset) {
  off64_t d_off = cookie_to_offset(d, offset);
  if (lseek(d->fd,d_off,SEEK_SET) != (off_t)-1) {
    d->num=d->cur=0;
    if (d->is_64) {
      struct linux_dirent64 *ld = (void *)(d->buf + d->cur);
      ld->d_off = d_off;
    } else {
      struct linux_dirent *ld = (void *)(d->buf + d->cur);
      ld->d_off = d_off;
    }
  }
}
