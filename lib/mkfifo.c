#include <linux/stat.h>
#include <unistd.h>

int mkfifo(const char *fn,mode_t mode) {
  return mknod(fn,S_IFIFO|mode,0);
}
