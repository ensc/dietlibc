#include <dietstdio.h>

static FILE __stdin = { 0, BUFINPUT, 0, 0 };

int __fflush_stdin(void) {
  return fflush(stdin);
}

FILE *stdin=&__stdin;
