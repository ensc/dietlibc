#define _LINUX_SOURCE
#include <unistd.h>
#include <errno.h>

int getentropy(void* buf,size_t buflen) {
  if (buflen>256) {
    errno=EIO;
    return -1;
  }
  if (getrandom(buf,buflen,GRND_NONBLOCK)<(int)buflen)
    return -1;
  return 0;
}
