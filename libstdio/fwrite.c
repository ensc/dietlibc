#include <sys/types.h>
#include "dietstdio.h"
#include <unistd.h>

size_t fwrite( const void *ptr, size_t size, size_t nmemb, FILE *stream) {
  int res;
#ifdef WANT_BUFFERED_STDIO
  long len=size*nmemb;
  long i;
  if (len>BUFSIZE || (stream->flags&NOBUF)) {
    if (!(stream->flags&NOBUF)) fflush(stream);
    res=write(stream->fd,ptr,size*nmemb);
  } else {
    register const unsigned char *c=ptr;
    for (i=size*nmemb; i>0; --i,++c)
      if (fputc(*c,stream)) {
	res=-1;
	break;
      }
    res=size*nmemb;
  }
#else
  res=write(stream->fd,ptr,size*nmemb);
#endif
  if (res<0) {
    stream->flags|=ERRORINDICATOR;
    return 0;
  }
  return res/size;
}
