#include <endian.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef __NO_STAT64
extern size_t __pread(int fd, void *buf, size_t count, off64_t a);

size_t __libc_pread64(int fd, void *buf, size_t count, off64_t offset);
size_t __libc_pread64(int fd, void *buf, size_t count, off64_t offset) {
  return __pread(fd,buf,count,offset);
}

int pread64(int fd, void *buf, size_t count, off_t offset) __attribute__((weak,alias("__libc_pread64")));
#endif
