#include <dietstdio.h>

#ifdef WANT_BUFFERED_STDIO
static FILE __stderr = { 2, 0, 0, 0 };

int __fflush_stderr() {
  return fflush(stderr);
}
#else
static FILE __stderr = { 2, 0 };
#endif

FILE *stderr=&__stderr;
