#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include "dietfeatures.h"

char *realpath(const char *path, char *resolved_path) {
  int fd=open(".",O_RDONLY);
  char *tmp=(char*)"";
  if (fd<0) return 0;
  if (chdir(path)) {
    if (errno==ENOTDIR)
    {
      if ((tmp=strrchr(path,'/'))) {
	memmove(resolved_path,path,tmp-path);
	resolved_path[tmp-path]=0;
	if (chdir(resolved_path)) { resolved_path=0; goto abort; }
      }
    }
  }
  if (!getcwd(resolved_path,PATH_MAX)) { resolved_path=0; goto abort; }
  strcat(resolved_path,tmp);
abort:
  fchdir(fd);
  close(fd);
  return resolved_path;
}
