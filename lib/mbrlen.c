#include <wchar.h>
#include <errno.h>

static mbstate_t internal;

size_t mbrlen(const char *s, size_t n, mbstate_t *ps) {
  /* glibc:
     static mbstate_t internal;
     return __mbrtowc (NULL, s, n, ps ?: &internal); */
  const char* o=s;
  if (!ps) ps=&internal;
  while (n) {
    --n;
    if (ps->count) {
      if ((*s&0xc0)!=0x80) {
      /* expected a continuation, didn't get one */
kaputt:
	errno=EILSEQ;
	ps->count=0;	/* susv3 says undefined; we reset */
	return (size_t)-1;
      }
      ps->sofar=(ps->sofar << 6) + (*s & 0x3f);
      if (!--ps->count) break;
    } else {
      if (*s&0x80) {
	unsigned char c=*s<<1;
	ps->count=0;
	while (c&0x80) {
	  c<<=1;
	  ++ps->count;
	}
	ps->sofar=c>>(ps->count+2);
	if (ps->count-1 > 4) goto kaputt;
      } else {
	ps->sofar=*s;
	break;
      }
    }
    ++s;
  }
  return s-o;
}
