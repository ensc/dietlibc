#include "dietstdio.h"

void clearerr( FILE *stream) {
  stream->flags&=~(ERRORINDICATOR|EOFINDICATOR);
}
