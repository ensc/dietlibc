#include <dietstdio.h>

int fputc_unlocked(int c, FILE *stream) {
  if (__fflush4(stream,0)) return EOF;
  if (stream->bm>=BUFSIZE-1)
    if (fflush(stream)) return EOF;
  stream->buf[stream->bm]=c;
  ++stream->bm;
  if (((stream->flags&BUFLINEWISE) && c=='\n') ||
      ((stream->flags&NOBUF))) /* puke */
    if (fflush(stream)) return EOF;
  return 0;
}

int fputc(int c,FILE* stream) __attribute__((weak,alias("fputc_unlocked")));
