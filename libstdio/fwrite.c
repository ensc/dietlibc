#include <sys/types.h>
#include "dietstdio.h"
#include <unistd.h>

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
  int res;
  long len=size*nmemb;
  long i;
  if (len>BUFSIZE || (stream->flags&NOBUF)) {
    fflush(stream);
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
  if (res<0) {
    stream->flags|=ERRORINDICATOR;
    return 0;
  }
  return size?res/size:0;
}
