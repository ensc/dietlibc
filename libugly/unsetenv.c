#include <stdlib.h>

void unsetenv(const char *name) {
  putenv(name);
}

