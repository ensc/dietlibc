#include <string.h>
#include <stdlib.h>

int setenv(const char *name, const char *value, int overwrite) {
  char *c=malloc(strlen(name)+strlen(value)+3);
  if (getenv(name)) {
    if (!overwrite) return 0;
    unsetenv(name);
  }
  strcpy(c,name);
  strcat(c,"=");
  strcat(c,value);
  return putenv(c);
}
