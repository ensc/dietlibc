#include <fcntl.h>
#include <dlfcn.h>
#include <limits.h>

int _dl_search(char*buf, int bufsize, const char*filename);
void *_dl_open(const char*pathname, int fd, int flag);

void *dlopen (const char *filename, int flag)
{
  int fd;
  char buf[PATH_MAX];
  const char *p;
  if (*filename=='/')
    fd=open(p=filename,O_RDONLY);
  else {
    p=buf;
    fd=_dl_search(buf,sizeof(buf),filename);
  }

  return _dl_open(p,fd,flag);
}
