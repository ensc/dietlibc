#include "dietfeatures.h"

#ifdef WANT_LARGEFILE_BACKCOMPAT
#include <sys/stat.h>
#ifndef __NO_STAT64
#include <errno.h>

extern int __dietlibc_stat64(const char *__file, struct stat64 *__buf);
extern void __stat64_cvt(const struct stat *src,struct stat64 *dest);

int stat64(const char *__file, struct stat64 *__buf) {
  if (__dietlibc_stat64(__file,__buf)) {
    struct stat temp;
#ifdef WANT_THREAD_SAVE
    if (*__errno_location()!=ENOSYS) return -1;
#else
    if (errno!=ENOSYS) return -1;
#endif
    if (stat(__file,&temp)) return -1;
    __stat64_cvt(&temp,__buf);
  }
  return 0;
}
#endif
#endif
