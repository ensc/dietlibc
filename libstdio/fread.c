#include <sys/types.h>
#include "dietstdio.h"
#include <unistd.h>

size_t fread( void *ptr, size_t size, size_t nmemb, FILE *stream) {
  int res;
  unsigned long i,j;
  j=size*nmemb;

  if (!j) return 0;
  if (stream->ungotten) {
    stream->ungotten=0;
    *(char*)ptr=stream->ungetbuf;
    ptr=((char*)ptr)+1;
    --j;
  }
  if (!j) return 1;

  for (i=0; i<j; ++i) {
    res=fgetc(stream);
    if (res==EOF)
      return i/size;
    else
      ((unsigned char*)ptr)[i]=(unsigned char)res;
  }
  return nmemb;
}
