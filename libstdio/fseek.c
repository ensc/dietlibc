#include <dietstdio.h>
#include <unistd.h>

int fseek(FILE *stream, long offset, int whence) {
  fflush(stream);
  stream->bm=0; stream->bs=0;
  stream->flags&=~(ERRORINDICATOR|EOFINDICATOR);
  return lseek(stream->fd,offset,whence)!=-1?0:-1;
}
