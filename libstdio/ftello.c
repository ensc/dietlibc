#include <dietstdio.h>
#include <unistd.h>

off_t ftello(FILE *stream) {
  fflush(stream);
  return (lseek(stream->fd,0,SEEK_CUR));
}
