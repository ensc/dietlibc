#include <unistd.h>
#include <stdlib.h>
#include "dietwarning.h"
#include "dietstdio.h"

FILE *__stdio_root;

int __stdio_atexit=0;

void __stdio_flushall(void);
void __stdio_flushall(void) {
  fflush(0);
}

extern int __fflush_stdin(void);
extern int __fflush_stdout(void);
extern int __fflush_stderr(void);

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
//  if (stream->flags&NOBUF) return 0;
  if (stream->flags&BUFINPUT) {
    register int tmp;
    if ((tmp=stream->bm-stream->bs)) {
      lseek(stream->fd,tmp,SEEK_CUR);
      stream->bs=0;
    }
  } else
    if (stream->bm && write(stream->fd,stream->buf,stream->bm)!=(int)stream->bm) {
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

int __buffered_outs(const char *s,size_t len) {
  return fwrite(s,1,(size_t)len,stdout);
}

link_warning("fflush","warning: your code uses stdio (7+k bloat).")
