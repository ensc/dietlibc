#include <sys/types.h>
#include "dietstdio.h"
#include <unistd.h>
#include "dietwarning.h"

int setvbuf(FILE *stream, char *buf, int flags , size_t size) {
  switch (flags) {
  case _IONBF: stream->flags = (stream->flags & ~(BUFLINEWISE)) | NOBUF; break;
  case _IOLBF: stream->flags = (stream->flags & ~(BUFLINEWISE|NOBUF)) | BUFLINEWISE; break;
  case _IOFBF: stream->flags = stream->flags & ~(NOBUF | BUFLINEWISE); break;
  default: return -1;
  }
  return 0;
}

link_warning("setvbuf","warning: setvbuf does not implement changing the buffer in diet libc.")
