#define _GNU_SOURCE
#include <stdlib.h>
#include <sys/auxv.h>

char* secure_getenv(const char* name) {
  if (getauxval(AT_SECURE))
    return NULL;
  return getenv(name);
}
