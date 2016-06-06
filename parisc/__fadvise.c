#include "syscalls.h"
#define _LINUX_SOURCE
#include <fcntl.h>
#include <unistd.h>

#ifndef __NR_fadvise64
long fadvise64_64(int fd, off64_t offset, off64_t len, int advice)
{
  extern long __parisc_fadvise64_64(int fd, long offset_hi, long offset_lo, long len_hi, long len_lo, int advice);

  return __parisc_fadvise64_64(fd, (long)(offset >> 32), (long)offset, (long)(len >> 32), (long)len, advice);
}

int posix_fadvise(int fd, off64_t offset, off64_t len, int advise)
  __attribute__((__alias__("fadvise64_64")));
#endif
