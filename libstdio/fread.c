#include <sys/types.h>
#include "dietstdio.h"
#include <unistd.h>

size_t fread( void *ptr, size_t size, size_t nmemb, FILE *stream) {
  int res;
  fflush(stream);
  res=read(stream->fd,ptr,size*nmemb);
  if (res<0) {
    stream->flags|=ERRORINDICATOR;
    return 0;
  } else if (res<size*nmemb)
    stream->flags|=EOFINDICATOR;
  return res/size;
}
