#include <dietstdio.h>

static FILE __stderr = { 2, NOBUF, 0, 0, 0 };

int __fflush_stderr(void) {
  return fflush(stderr);
}

FILE *stderr=&__stderr;
