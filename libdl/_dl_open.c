#include <fcntl.h>
#include <dlfcn.h>
#include <limits.h>
#include <string.h>

#include "_dl_int.h"

void *_dl_open(const char *filename, int flags)
{
  int fd;
  char buf[PATH_MAX];
  const char *p;

  _dl_error_location="dlopen";
  _dl_error_data=filename;
  _dl_error=0;

  if (strchr(filename,'/'))
    fd=open(p=filename,O_RDONLY);
  else {
    p=buf;
    fd=_dl_search(buf,sizeof(buf)-1,filename);
  }
  if (fd==-1) return 0;
  return _dl_load(filename,p,fd,flags);
}
