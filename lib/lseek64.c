#include "dietfeatures.h"

#ifdef WANT_LARGEFILE_BACKCOMPAT
#include <sys/stat.h>
#ifndef __NO_STAT64
#include <errno.h>
#include <unistd.h>

loff_t lseek64(int fildes, loff_t offset, int whence) {
  loff_t tmp;
  if (llseek(fildes,offset>>32,offset&0xffffffff,&tmp,whence)) {
#ifdef WANT_THREAD_SAVE
    if (*__errno_location()!=ENOSYS) return -1;
#else
    if (errno!=ENOSYS) return -1;
#endif
    return (loff_t)lseek(fildes,(off_t)offset,whence);
  }
  return tmp;
}
#endif
#endif
