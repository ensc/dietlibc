#include <fcntl.h>
#include <dlfcn.h>
#include <limits.h>

#include "_dl_int.h"

void *dlopen (const char *filename, int flag)
{
  int fd;
  char buf[PATH_MAX];
  const char *p;

  _dl_error_location="dlopen";
  _dl_error_data=filename;
  _dl_error=0;

  if (filename) {
    if (*filename=='/')
      fd=open(p=filename,O_RDONLY);
    else {
      p=buf;
      fd=_dl_search(buf,sizeof(buf),filename);
    }
    return _dl_open(filename,p,fd,flag);
  }
  /* dietld.so has allocated the top for the dynamic program.
   * (if there is a dietld.so :) not yet functional
   * (started the implementation) )*/
  return _dl_root_handle;
}
