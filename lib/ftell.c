#include <dietstdio.h>
#include <unistd.h>

long ftell(FILE *stream) {
  fflush(stream);
  return (lseek(stream->fd,0,SEEK_CUR));
}
