#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#undef _POSIX_PTHREADS
#include "dirstream.h"

#undef closedir
int closedir(DIR * dir)
{
  int fd;

  if (!dir) {
    errno = EBADF;
    return -1;
  }

  /* We need to check dd_fd. */
  if (dir->dd_fd == -1)
  {
    errno = EBADF;
    return -1;
  }
  fd = dir->dd_fd;
  dir->dd_fd = -1;
  free(dir->dd_buf);
  free(dir);
  return close(fd);
}
