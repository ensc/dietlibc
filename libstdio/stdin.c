#include <dietstdio.h>

static char __stdin_buf[BUFSIZE];
static FILE __stdin = {
  .fd=0,
  .flags=BUFINPUT|STATICBUF,
  .bs=0, .bm=0,
  .buflen=BUFSIZE,
  .buf=__stdin_buf,
  .next=0,
  .popen_kludge=0,
  .ungetbuf=0,
  .ungotten=0
#ifdef WANT_THREAD_SAFE
  , .m=PTHREAD_MUTEX_INITIALIZER
#endif
};

FILE *stdin=&__stdin;

int __fflush_stdin(void) {
  return fflush(stdin);
}
