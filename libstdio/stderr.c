#include <dietstdio.h>

static FILE __stderr = { 2, NOBUF, 0, 0, 0 };

FILE *stderr=&__stderr;

int __fflush_stderr(void) {
  return fflush(stderr);
}
