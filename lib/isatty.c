#define ioctl libc_ioctl
#include <termios.h>
#undef ioctl
#include <sys/ioctl.h>
#include <errno.h>
#include "dietfeatures.h"

extern int errno;

int isatty(int fd)
{
  int save;
  int is_tty;
  struct termios term;

#ifdef WANT_THREAD_SAFE
  int *err = __errno_location();
  save = *err;
  is_tty = ioctl(fd, TCGETS, &term) == 0;
  *err = save;
#else
  save = errno;
  is_tty = ioctl(fd, TCGETS, &term) == 0;
  errno = save;
#endif

  return is_tty;
}

