#include "dietfeatures.h"
#include "dietdirent.h"
#include "syscalls.h"
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#if defined(WANT_LARGEFILE_BACKCOMPAT)
int getdents64(int fd, struct dirent64 *dirp, unsigned int count) {
  static int trygetdents64=1;
  struct dirent o;
  int res;
#ifdef __NR_getdents64
  if (trygetdents64) {
    int res=getdents64(fd,dirp,count);
    if (res==-1 && errno==ENOSYS)
      trygetdents64=0;
    else
      return res;
  }
#endif
  res=getdents(fd,&o,count);
  if (res!=-1) {
    dirp->d_ino=o.d_ino;
    dirp->d_off=o.d_off;
    dirp->d_reclen=o.d_reclen;
    strcpy(dirp->d_name,o.d_name);
    dirp->d_type=0;
  }
  return res;
}
#endif
