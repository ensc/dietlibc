#include <unistd.h>
#include <errno.h>

#include "dirstream.h"

/*
 * rewinddir() just does an lseek(fd,0,0) - see close for comments
 */
void rewinddir(DIR * dir)
{
  if (!dir) {
    errno = EBADF;
    return;
  }
  lseek(dir->dd_fd,0,SEEK_SET);
  dir->dd_nextoff = dir->dd_nextloc = dir->dd_size = 0;
}
