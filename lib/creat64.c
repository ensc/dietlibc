#include <fcntl.h>

#ifdef O_LARGEFILE

int creat64(const char *file,mode_t mode) {
  return open(file,O_WRONLY|O_CREAT|O_TRUNC|O_LARGEFILE,mode);
}
#endif
