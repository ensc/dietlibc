#include <dietstdio.h>

int feof(FILE *stream) {
#ifdef WANT_UNGETC
  /* yuck!!! */
  if (stream->ungotten) return 0;
#endif
  return (stream->flags&EOFINDICATOR);
}
