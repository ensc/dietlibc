#include "dietfeatures.h"
#include "dietdirent.h"
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>

#ifndef WANT_LARGEFILE_BACKCOMPAT
struct dirent64* readdir64(DIR *d) {
  if (!d->num || (d->cur += ((struct dirent64*)(d->buf+d->cur))->d_reclen)>=d->num) {
    int res=getdents64(d->fd,(struct dirent64*)d->buf,1023);
    if (res<=0) return 0;
    d->num=res; d->cur=0;
  }
  return (struct dirent64*)(d->buf+d->cur);
}
#else
struct dirent64* readdir64(DIR *d) {
  static int trygetdents64=1;
  struct dirent* o;
  static struct dirent64 d64;
again:
  if (!trygetdents64) {
    if (!d->num || (d->cur += ((struct dirent*)(d->buf+d->cur))->d_reclen)>=d->num) {
      int res=getdents(d->fd,(struct dirent*)d->buf,1023);
      if (res<=0) return 0;
      d->num=res; d->cur=0;
    }
    o=(struct dirent*)(d->buf+d->cur);
    d64.d_ino=o->d_ino;
    d64.d_off=o->d_off;
    d64.d_reclen=o->d_reclen;
    strcpy(d64.d_name,o->d_name);
    d64.d_type=0;	/* is this correct? */
    return &d64;
  }
  if (!d->num || (d->cur += ((struct dirent64*)(d->buf+d->cur))->d_reclen)>=d->num) {
    int res=getdents64(d->fd,(struct dirent64*)d->buf,1023);
    if (res<=0) {
      if (errno==ENOSYS) {
	trygetdents64=0;
	goto again;
      }
      return 0;
    }
    d->num=res; d->cur=0;
  }
  return (struct dirent64*)(d->buf+d->cur);
}
#endif
