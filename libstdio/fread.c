#include <sys/types.h>
#include "dietstdio.h"
#include <unistd.h>

size_t fread( void *ptr, size_t size, size_t nmemb, FILE *stream) {
  int res;
#ifdef WANT_BUFFERED_STDIO
  unsigned long i,j;
  j=size*nmemb;
#ifdef WANT_UNGETC
  if (!j) return 0;
  if (stream->ungotten) {
    stream->ungotten=0;
    *(char*)ptr=stream->ungetbuf;
    ptr=((char*)ptr)+1;
    --j;
  }
  if (!j) return 1;
#endif
  for (i=0; i<j; ++i) {
    res=fgetc(stream);
    if (res==EOF)
      return i/size;
    else
      ((unsigned char*)ptr)[i]=(unsigned char)res;
  }
  return nmemb;
#else
  unsigned long j=size*nmemb;
  fflush(stream);
#ifdef WANT_UNGETC
  if (stream->ungotten) {
    stream->ungotten=0;
    *(char*)ptr=stream->ungetbuf;
    ptr=((char*)ptr)+1;
    --j;
  }
  if (!j) return 1;
#endif
  while (j>0) {
    res=read(stream->fd,ptr,j);
    if (res<0) {
      stream->flags|=ERRORINDICATOR;
      return 0;
    } else if (res==0) {
      stream->flags|=EOFINDICATOR;
      break;
    }
    j-=res;
    ptr=((char*)ptr)+res;
  }
  return res/size;
#endif
}
