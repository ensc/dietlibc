#include <dietstdio.h>

int feof(FILE *stream) {
  /* yuck!!! */
  if (stream->ungotten) return 0;
  return (stream->flags&EOFINDICATOR);
}
