#include <dietstdio.h>
#include <unistd.h>
#include <errno.h>

long ftell(FILE *stream) {
  off_t l;
  fflush(stream);
  l=lseek(stream->fd,0,SEEK_CUR);
  if (l==-1) return -1;
  l-=stream->ungotten;
  {
    long x=l;
    if (x!=l) {
      errno=EOVERFLOW;
      return -1;
    }
    return x;
  }
}
