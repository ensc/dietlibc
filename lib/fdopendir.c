#include "dietdirent.h"
#include "dietpagesize.h"
#include <sys/mman.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <fcntl.h>

DIR*  fdopendir ( int fd ) {
  DIR*  t  = NULL;

  if ( fd >= 0 ) {
    t = (DIR *) mmap (NULL, __DIET_PAGE_SIZE, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (t != MAP_FAILED)
      t->fd = fd;
  }


  return t;
}
