#include "dietstdio.h"
#include <stdlib.h>
#include <unistd.h>

int fclose(FILE *stream) {
  int res;
  FILE *f,*fl;
  fflush(stream);
  res=close(stream->fd);
  for (fl=0,f=__stdio_root; f; fl=f,f=f->next)
    if (f==stream) {
      if (fl)
	fl->next=f->next;
      else
	__stdio_root=f->next;
      break;
    }
  if ((!(stream->flags&STATICBUF))&&(stream->buf))
    free(stream->buf);
  free(stream);
  return res;
}
