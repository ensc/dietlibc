#include <dietstdio.h>
#include <unistd.h>

long fseek(FILE *stream, long offset, int whence) {
  fflush(stream);
#ifdef WANT_BUFFERED_STDIO
  stream->bm=0; stream->bs=0;
#endif
  stream->flags&=~(ERRORINDICATOR|EOFINDICATOR);
  return (lseek(stream->fd,offset,whence));
}
