#include <sys/types.h>
#include "dietstdio.h"
#include <unistd.h>
#include <stdlib.h>
#include "dietwarning.h"

int setvbuf(FILE *stream, char *buf, int flags , size_t size) {
  stream->flags|=STATICBUF;
  if (buf) {
    if (!(stream->flags&STATICBUF)) free(stream->buf);
    stream->buf=buf;
  }
  else {
    char *tmp;
    if (!(tmp=malloc(size))) return -1;
    if (!(stream->flags&STATICBUF)) free(stream->buf);
    stream->buf=tmp;
  }
  stream->buflen=size;
  stream->bm=stream->bs=0;
  switch (flags) {
  case _IONBF: stream->flags = (stream->flags & ~(BUFLINEWISE)) | NOBUF; break;
  case _IOLBF: stream->flags = (stream->flags & ~(BUFLINEWISE|NOBUF)) | BUFLINEWISE; break;
  case _IOFBF: stream->flags = stream->flags & ~(NOBUF | BUFLINEWISE); break;
  default: return -1;
  }
  return 0;
}
