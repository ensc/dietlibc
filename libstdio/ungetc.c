#include "dietstdio.h"

int ungetc(int c, FILE *stream) {
  if (stream->ungotten)
    return EOF;
  stream->ungotten=1;
  stream->ungetbuf=(char)(unsigned char)c;
  return c;
}
