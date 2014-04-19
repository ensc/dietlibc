#include "syscalls.h"

#define _LINUX_SOURCE
#include <fcntl.h>
#include <unistd.h>

struct fadvise64_64_layout
{
  int fd;
  off64_t offset;
  off64_t len;
  int advise;
};

long fadvise64_64(int fd, off64_t offset, off64_t len, int advise)
{
  struct fadvise64_64_layout parameters;
  extern long __s390_fadvise64_64(struct fadvise64_64_layout *args);

  parameters.fd = fd;
  parameters.offset = offset;
  parameters.len = len;
  parameters.advise = advise;

  return __s390_fadvise64_64(&parameters);
}

int posix_fadvise(int fd, off64_t offset, off64_t len, int advise)
  __attribute__((__alias__("fadvise64_64")));
