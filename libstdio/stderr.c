#include <dietstdio.h>

#ifdef WANT_BUFFERED_STDIO
static FILE __stderr = { 2, NOBUF, 0, 0 };

int __fflush_stderr(void) {
  return fflush(stderr);
}
#else
static FILE __stderr = { 2, 0 };
#endif

FILE *stderr=&__stderr;
