#include <dietstdio.h>

int fileno(FILE *stream) {
  return stream->fd;
}
