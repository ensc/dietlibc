#include <dietstdio.h>

int ferror(FILE *stream) {
  return (stream->flags&ERRORINDICATOR);
}
