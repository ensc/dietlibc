#include <dietstdio.h>
#include <unistd.h>

off_t fseeko(FILE *stream, off_t offset, int whence) {
  fflush(stream);
#ifdef WANT_BUFFERED_STDIO
  stream->bm=0; stream->bs=0;
#endif
  stream->flags&=~(ERRORINDICATOR|EOFINDICATOR);
  return (lseek(stream->fd,offset,whence));
}
