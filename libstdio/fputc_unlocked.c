#include <dietstdio.h>
#ifndef WANT_BUFFERED_STDIO
#include <unistd.h>
#endif

int fputc_unlocked(int c, FILE *stream) {
#ifdef WANT_BUFFERED_STDIO
  if (__fflush4(stream,0)) return EOF;
  if (stream->bm>=BUFSIZE-1)
    if (fflush(stream)) return EOF;
  stream->buf[stream->bm]=c;
  ++stream->bm;
  if (((stream->flags&BUFLINEWISE) && c=='\n') ||
      ((stream->flags&NOBUF))) /* puke */
    if (fflush(stream)) return EOF;
#else
  char ch=c;
  write(stream->fd,&ch,1);
#endif
  return 0;
}

int fputc(int c,FILE* stream) __attribute__((weak,alias("fputc_unlocked")));
