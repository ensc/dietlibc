#include <fcntl.h>

int creat(const char *file,mode_t mode) {
  return open(file,O_WRONLY|O_CREAT|O_TRUNC,mode);
}
