#define _GNU_SOURCE
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void verr(int e,const char* f,va_list ap) {
  vwarn(f,ap);
  exit(e);
}
