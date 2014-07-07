#include "dietstdio.h"

char *fgets_unlocked(char *s, int size, FILE *stream) {
  int l;
  if (size<=0) return 0;
  for (l=0; l+1<size; ) {
    register int c;
    if (l && __likely(stream->bm<stream->bs)) {
      /* try common case first */
      c=(unsigned char)stream->buf[stream->bm++];
    } else {
      c=fgetc_unlocked(stream);
      if (__unlikely(c==EOF)) {
	if (!l) return 0;
	break;
      }
    }
    s[l]=c;
    ++l;
    if (c=='\n') break;
  }
  s[l]=0;
  return s;
}

char*fgets(char*s,int size,FILE*stream) __attribute__((weak,alias("fgets_unlocked")));
