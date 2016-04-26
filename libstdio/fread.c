#include <sys/types.h>
#include "dietstdio.h"
#include <unistd.h>

size_t fread_unlocked(void *ptr, size_t size, size_t nmemb, FILE *stream) {
  unsigned long i,j;
  j=size*nmemb;
  i=0;

  if (!(stream->flags&CANREAD)) {
    stream->flags|=ERRORINDICATOR;
    return 0;
  }
  if (!j || j/nmemb!=size) return 0;
  if (__unlikely(stream->ungotten)) {
    stream->ungotten=0;
    *(char*)ptr=stream->ungetbuf;
    ++i;
    if (j==1) return 1;
  }

#ifdef WANT_FREAD_OPTIMIZATION
  size_t inbuf=stream->bs-stream->bm;
  if (__unlikely(!inbuf)) {
    stream->bm=0;
    ssize_t res=__libc_read(stream->fd,stream->buf,stream->buflen);
    if (res<1) {
      stream->flags |= (res==0 ? EOFINDICATOR : ERRORINDICATOR);
      return i/size;
    }
    inbuf=stream->bs=res;
  }
  if (inbuf) {
    size_t tocopy=j>inbuf ? inbuf : j;
    memcpy(ptr+i,stream->buf+stream->bm,tocopy);
    i+=tocopy;
    stream->bm+=tocopy;
    if (stream->bm==stream->bs)
      stream->bm=stream->bs=0;
    if (i==j) return nmemb;
  }
  if ( !(stream->flags&FDPIPE) && (j>stream->buflen)) {
    size_t tmp=j-i;
    ssize_t res;
//    if (fflush_unlocked(stream)) return 0;
    while ((res=__libc_read(stream->fd,ptr+i,tmp))<(ssize_t)tmp) {
      if (res==-1) {
	stream->flags|=ERRORINDICATOR;
	goto exit;
      } else if (!res) {
	stream->flags|=EOFINDICATOR;
	goto exit;
      }
      i+=res; tmp-=res;
    }
    return nmemb;
  }
#endif
  for (; i<j; ++i) {
    int res;
    res=fgetc_unlocked(stream);
    if (res==EOF)
exit:
      return i/size;
    else
      ((unsigned char*)ptr)[i]=(unsigned char)res;
  }
  return nmemb;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) __attribute__((weak,alias("fread_unlocked")));
