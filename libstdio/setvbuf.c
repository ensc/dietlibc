#include <sys/types.h>
#include "dietstdio.h"
#include <unistd.h>
#include "dietwarning.h"

int setvbuf(FILE *stream, char *buf, int flags , size_t size) {
#ifdef WANT_BUFFERED_STDIO
  switch (flags) {
  case _IONBF: stream->flags = (stream->flags & ~(BUFLINEWISE)) | NOBUF; break;
  case _IOLBF: stream->flags = (stream->flags & ~(BUFLINEWISE|NOBUF)) | BUFLINEWISE; break;
  case _IOFBF: stream->flags = stream->flags & ~(NOBUF | BUFLINEWISE); break;
  default: return -1;
  }
#endif
  return 0;
}

link_warning("setvbuf","setvbuf does not implement changing the buffer in diet libc.")
