#include <dietstdio.h>

int feof(FILE *stream) {
  return (stream->flags&EOFINDICATOR);
}
