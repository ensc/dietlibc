#include "syscalls.h"
#define _LINUX_SOURCE
#include <fcntl.h>
#include <unistd.h>

long posix_fadvise(int fd, off64_t offset, off64_t len, int advise)
    __attribute__((__alias__("fadvise64_64")));

long fadvise64_64(int fd, off64_t offset, off64_t len, int advise)
{
  extern long __ppc_fadvise64_64(int fd, int advise, long long off, long long len);
  return __ppc_fadvise64_64(fd, advise, (long long)offset, (long long)len);
}
