#include <dietstdio.h>

static char __stdout_buf[BUFSIZE];
static FILE __stdout = { 1, BUFLINEWISE|STATICBUF, 0, 0, BUFSIZE, __stdout_buf };

FILE *stdout=&__stdout;

int __fflush_stdout(void) {
  return fflush(stdout);
}
