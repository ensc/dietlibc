#include <fcntl.h>
#include <dlfcn.h>
#include <limits.h>

#include "_dl_int.h"

void *dlopen (const char *filename, int flag)
{
  int fd;
  char buf[PATH_MAX];
  const char *p;
  if (filename) {
    if (*filename=='/')
      fd=open(p=filename,O_RDONLY);
    else {
      p=buf;
      fd=_dl_search(buf,sizeof(buf),filename);
    }
    return _dl_open(p,fd,flag);
  }
  return _dl_root_handle;
}
