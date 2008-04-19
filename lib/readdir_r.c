#define _POSIX_SOURCE
#include "dietdirent.h"
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>

int readdir_r(DIR *d,struct dirent* entry, struct dirent** result) {
  struct linux_dirent* ld = d->num ? (struct linux_dirent*)(d->buf+d->cur) : NULL;
  *result=0;
  if (!d->num || d->cur >= d->num || (d->cur += ld->d_reclen)>=d->num) {
    int res=getdents(d->fd,(struct linux_dirent*)d->buf,__DIRSTREAM_BUF_SIZE-1);
    if (res<=0)
      return res<0;
    d->num=res; d->cur=0; d->is_64=0;
  }
  ld=(struct linux_dirent*)(d->buf+d->cur);
  if (ld->d_reclen < sizeof(struct linux_dirent))
    return 0;	/* can't happen */
  *result=entry;
  entry->d_ino=ld->d_ino;
  entry->d_off=ld->d_off;
  entry->d_reclen=ld->d_reclen;
  entry->d_type=ld->d_name[ld->d_reclen-offsetof(struct linux_dirent,d_name)-1];
  memcpy(entry->d_name,ld->d_name,ld->d_reclen-offsetof(struct linux_dirent,d_name));
  return 0;
}
