#define _GNU_SOURCE
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void errx(int e,const char* f, ...) {
  va_list ap;
  va_start(ap,f);
  vwarnx(f,ap);
  va_end(ap);
  exit(e);
}
