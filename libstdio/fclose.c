#include "dietstdio.h"
#include <stdlib.h>
#include <unistd.h>

int fclose(FILE *stream) {
  int res;
#ifdef WANT_BUFFERED_STDIO
  FILE *f,*fl;
#endif
  fflush(stream);
  res=close(stream->fd);
#ifdef WANT_BUFFERED_STDIO
  for (fl=0,f=__stdio_root; f; fl=f,f=f->next)
    if (f==stream) {
      if (fl)
	fl->next=f->next;
      else
	__stdio_root=f->next;
      break;
    }
#endif
  free(stream);
  return res;
}
