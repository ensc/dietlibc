#include <linux/types.h>
#include <linux/unistd.h>

#define __NR__mmap __NR_mmap
_syscall1(int,_mmap,unsigned long *,buffer)

char *mmap(char *addr, size_t len, int prot, int flags, int fd, unsigned long off) {
  unsigned long buffer[6];
  buffer[0] = (unsigned long)addr;
  buffer[1] = (unsigned long)len;
  buffer[2] = (unsigned long)prot;
  buffer[3] = (unsigned long)flags;
  buffer[4] = (unsigned long)fd;
  buffer[5] = (unsigned long)off;
  return (char*) _mmap(buffer);
}
