#include <dietstdio.h>

static char __stdin_buf[BUFSIZE];
static FILE __stdin = { 0, BUFINPUT|STATICBUF, 0, 0, BUFSIZE, __stdin_buf };

FILE *stdin=&__stdin;

int __fflush_stdin(void) {
  return fflush(stdin);
}
