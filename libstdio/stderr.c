#include <dietstdio.h>

static FILE __stderr = {
  .fd=2,
  .flags=NOBUF,
  .bs=0, .bm=0,
  .buflen=0,
  .buf=0,
  .next=0,
  .popen_kludge=0,
  .ungetbuf=0,
  .ungotten=0
#ifdef WANT_THREAD_SAFE
  , .m=PTHREAD_MUTEX_INITIALIZER
#endif
};

FILE *stderr=&__stderr;

int __fflush_stderr(void) {
  return fflush(stderr);
}
