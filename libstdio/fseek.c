#include <dietstdio.h>
#include <unistd.h>

int fseek(FILE *stream, long offset, int whence) {
  fflush(stream);
  return (lseek(stream->fd,offset,whence));
}
