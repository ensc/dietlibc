#include "dietdirent.h"
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>

struct dirent* __dietreaddir(DIR *d) {
  if (d->cur>=d->num) {
    int res=getdents(d->fd,(struct dirent*)d->buf,1023);
    if (res<=0) return 0;
    d->num=res; d->cur=0;
  } else
    d->cur+=((struct dirent*)(d->buf+d->cur))->d_reclen;
  return (struct dirent*)(d->buf+d->cur);
}
