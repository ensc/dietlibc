#include <sys/stat.h>

#undef stat
extern int stat(const char* fn,void *buf);

int __xstat(int ver,char* filename,struct stat* buf) {
  return stat(filename,buf);
}

