#include <sys/stat.h>

int __xmknod(int ver,char* filename,mode_t mode,dev_t *dev) {
  return mknod(filename,mode,*dev);
}

