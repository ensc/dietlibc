#include <unistd.h>
#include <errno.h>

#include "dirstream.h"

#undef telldir
off_t
telldir(DIR * dir)
{
  off_t offset;

  if (!dir) {
    errno = EBADF;
    return -1;
  }

  switch (dir->dd_getdents)
  {
  case no_getdents:
    /* We are running the old kernel. This is the starting offset
       of the next readdir(). */
    offset = lseek(dir->dd_fd, 0, SEEK_CUR);
    break;

  case unknown:
    /* readdir () is not called yet. but seekdir () may be called. */
  case have_getdents:
    /* The next entry. */
    offset = dir->dd_nextoff;
    break;

  default:
    errno = EBADF;
    offset = -1;
  }

  return offset;
}
