#include <dietstdio.h>
#include <unistd.h>

#ifndef __NO_STAT64
off64_t ftello64(FILE *stream) {
  fflush(stream);
  return (lseek64(stream->fd,0,SEEK_CUR));
}
#endif
