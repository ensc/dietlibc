#include <sys/stat.h>

#undef fstat
extern int fstat(int fd,void *buf);

int __fxstat(int ver,int fd,struct stat* buf) {
  return fstat(fd,buf);
}

