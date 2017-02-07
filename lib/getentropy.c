#include <sys/random.h>
#include <errno.h>
#include <fcntl.h>

int getentropy(void* buf,size_t buflen) {
  int r;
  if (buflen>256) {
    errno=EIO;
    return -1;
  }
  r=getrandom(buf,buflen,GRND_NONBLOCK);
  if (r==-1 && errno==ENOSYS) {
    int fd=open("/dev/urandom",O_RDONLY);
    if (fd==-1) return -1;
    r=read(fd,buf,buflen);
    close(fd);
  }
  if (r<(int)buflen)
    return -1;
  return 0;
}
