#include <dietstdio.h>

#ifdef WANT_BUFFERED_STDIO
static FILE __stdin = { 0, BUFINPUT, 0, 0 };

int __fflush_stdin(void) {
  return fflush(stdin);
}
#else
static FILE __stdin = { 0, 0 };
#endif

FILE *stdin=&__stdin;
