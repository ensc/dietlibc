#include <dietstdio.h>

static FILE __stdout = { 1, BUFLINEWISE, 0, 0 };

int __fflush_stdout(void) {
  return fflush(stdout);
}

FILE *stdout=&__stdout;

