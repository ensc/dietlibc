#if 0

Sorry, currently broken

#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "dirstream.h"

#ifdef __ELF__
#pragma weak readdir = __libc_readdir
#endif

struct dirent *
__libc_readdir(DIR * dir)
{
  int result;
  struct dirent *de;

  if (!dir) {
    errno = EBADF;
    return NULL; 
  }

  /* Are we running an old kernel? */
  if (dir->dd_getdents == no_getdents)
  {
    abort();
  }

  if (dir->dd_size <= dir->dd_nextloc)
  {
    /* read dir->dd_max bytes of directory entries. */
    result=getdents(dir->dd_fd,dir->dd_buf,dir->dd_max);

    /* We assume we have getdents (). */
    dir->dd_getdents = have_getdents;
    if (result <= 0)
    {
      result = -result;
      if (result > 0)
      {
	/* Are we right? */
	if (result == ENOSYS)
	{
	  dir->dd_getdents = no_getdents;
	  abort();
	}
	errno = result;
      }

      return NULL;
    }

    dir->dd_size = result;
    dir->dd_nextloc = 0;
  }

  de = (struct dirent *) (((char *)dir->dd_buf) + dir->dd_nextloc);

  /* Am I right? H.J. */
  dir->dd_nextloc += de->d_reclen;

  /* We have to save the next offset here. */
  dir->dd_nextoff = de->d_off;

  /* convert */
  dir->convbuf.d_ino=de->d_ino;
  dir->convbuf.d_off=de->d_off;
  dir->convbuf.d_reclen=de->d_reclen;
  dir->convbuf.d_type=0;
  if (strlen((char*)&de->d_type)>10)
    de->d_name[10]=0;
  strcpy(dir->convbuf.d_name,(char*)&de->d_type);
  errno=0;

  return &dir->convbuf;
}

#endif
