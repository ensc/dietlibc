#include "dietstdio.h"
#include <unistd.h>

extern int feof(FILE *stream);

int fgetc_unlocked(FILE *stream) {
  unsigned char c;
#ifdef WANT_UNGETC
  if (stream->ungotten) {
    stream->ungotten=0;
    return stream->ungetbuf;
  }
#endif
  if (feof(stream))
    return EOF;
#ifdef WANT_BUFFERED_STDIO
  if (__fflush4(stream,BUFINPUT)) return EOF;
  if (stream->bm>=stream->bs) {
    int len=read(stream->fd,stream->buf,BUFSIZE);
    if (len==0) {
      stream->flags|=EOFINDICATOR;
      return EOF;
    } else if (len<0) {
      stream->flags|=ERRORINDICATOR;
      return EOF;
    }
    stream->bm=0;
    stream->bs=len;
  }
  c=stream->buf[stream->bm];
  ++stream->bm;
  return c;
#else
  if (read(stream->fd,&c,1)!=1) {
    stream->flags|=ERRORINDICATOR;
    return EOF;
  }
  return c;
#endif
}

#ifndef WANT_THREAD_SAFE
int fgetc(FILE* stream) __attribute__((weak,alias("fgetc_unlocked")));
#endif
