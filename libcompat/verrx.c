#define _GNU_SOURCE
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void verrx(int e,const char* f,va_list ap) {
  vwarnx(f,ap);
  exit(e);
}
