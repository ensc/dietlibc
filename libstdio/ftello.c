#include <dietstdio.h>
#include <unistd.h>

off_t ftello_unlocked(FILE *stream) {
  fflush(stream);
  return (lseek(stream->fd,0,SEEK_CUR));
}

off_t ftello(FILE *stream) __attribute__((weak,alias("ftello_unlocked")));
