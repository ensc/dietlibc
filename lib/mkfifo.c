#include <linux/stat.h>
#include <unistd.h>

int mkfifo(char *fn,int mode) {
  return mknod(fn,S_IFIFO|mode,0);
}
