#include <sys/types.h>
#include <sys/socket.h>
#include <linuxnet.h>
ssize_t __libc_send(int fd, const void * buf, size_t n, int flags);
  /* shut up gcc warning about missing prototype */

ssize_t __libc_send(int fd, const void * buf, size_t n, int flags) {
  return sendto(fd, buf, n, flags, NULL, 0);
}

ssize_t send(int a, const void * b, size_t c, int flags)
  __attribute__ ((weak, alias("__libc_send")));
