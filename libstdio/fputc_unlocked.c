#include <dietstdio.h>
#include <unistd.h>

int fputc_unlocked(int c, FILE *stream) {
  if (__fflush4(stream,0)) return EOF;
  if (stream->bm>=stream->buflen-1)
    if (fflush(stream)) return EOF;
  if (stream->flags&NOBUF) {
    if (write(stream->fd,&c,1) != 1) return EOF;
    return 0;
  }
  stream->buf[stream->bm]=c;
  ++stream->bm;
  if (((stream->flags&BUFLINEWISE) && c=='\n') ||
      ((stream->flags&NOBUF))) /* puke */
    if (fflush(stream)) return EOF;
  return 0;
}

int fputc(int c,FILE* stream) __attribute__((weak,alias("fputc_unlocked")));
