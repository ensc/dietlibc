#include <dietstdio.h>
#include <unistd.h>

#ifndef __NO_STAT64
off64_t ftello64_unlocked(FILE *stream) {
  fflush_unlocked(stream);
  return (lseek64(stream->fd,0ull,SEEK_CUR));
}

off64_t ftello64(FILE *stream) __attribute__((weak,alias("ftello64_unlocked")));
#endif
