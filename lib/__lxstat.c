#include <sys/stat.h>

#undef lstat
extern int lstat(const char* fn,void *buf);

int __lxstat(int ver,char* filename,struct stat* buf) {
  return lstat(filename,buf);
}

