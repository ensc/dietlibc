#include <dietstdio.h>

#ifdef WANT_BUFFERED_STDIO
static FILE __stdout = { 1, 0, 0, 0 };

int __fflush_stdout() {
  return fflush(stdout);
}
#else
static FILE __stdout = { 1, 0 };
#endif

FILE *stdout=&__stdout;

