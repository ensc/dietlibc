#include "dietstdio.h"
#include <unistd.h>
#include <stdlib.h>
#include "dietwarning.h"

FILE *__stdio_root;

#ifdef WANT_BUFFERED_STDIO
int __stdio_atexit=0;

void __stdio_flushall() {
  fflush(0);
}

extern int __fflush_stdin();
extern int __fflush_stdout();
extern int __fflush_stderr();

int fflush(FILE *stream) {
  if (stream==0) {
    int res;
    FILE *f;
    __fflush_stdin();
    __fflush_stdout();
    __fflush_stderr();
    for (res=0, f=__stdio_root; f; f=f->next)
      if (fflush(f))
	res=-1;
    return res;
  }
  if (stream->flags&BUFINPUT) {
    register int tmp;
    if ((tmp=stream->bm-stream->bs)) lseek(stream->fd,tmp,SEEK_CUR);
  } else
    if (stream->bm && write(stream->fd,stream->buf,stream->bm)!=stream->bm) {
      stream->flags|=ERRORINDICATOR;
      return -1;
    }
  stream->bm=0;
  return 0;
}

int __fflush4(FILE *stream,int next) {
  if (!__stdio_atexit) {
    __stdio_atexit=1;
    atexit(__stdio_flushall);
  }
  if ((stream->flags&BUFINPUT)!=next) {
    int res=fflush(stream);
    stream->flags=(stream->flags&~BUFINPUT)|next;
    return res;
  }
  return 0;
}

#else
int fflush(FILE *stream) {
  return 0;
}
#endif

link_warning("fflush","warning: your code uses stdio (several kilobytes of bloat).")
