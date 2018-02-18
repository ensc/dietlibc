#define _FILE_OFFSET_BITS       64

#include "dietdirent.h"
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

#ifdef __DIR_OFFSET_COOKIE
static long offset_to_cookie(DIR *d, off64_t offset)
{
  size_t i;
  struct __dirent64_offsets *tmp;

  for (i = 0; i < d->num_offsets; ++i) {
    if (d->offsets[i].d_off == offset)
      return i;
  }

  tmp = reallocarray(d->offsets, d->num_offsets + 1, sizeof d->offsets[0]);
  if (!tmp)
    /* ENOMEM */
    return -1;

  tmp[d->num_offsets].d_off = offset;
  d->offsets = tmp;
  ++d->num_offsets;

  return d->num_offsets - 1;
}
#else
static long offset_to_cookie(DIR *d, off64_t offset)
{
  return offset;
}
#endif

long telldir(DIR *d) {
  long result = 0;
  if (lseek(d->fd,0,SEEK_CUR) != (off_t)-1) {
    if (d->is_64) {
      struct linux_dirent64 *ld = (void *)(d->buf + d->cur);
      result = offset_to_cookie(d, ld->d_off);
    } else {
      struct linux_dirent *ld = (void *)(d->buf + d->cur);
      result=ld->d_off;
    }
  }
  return result;
}
