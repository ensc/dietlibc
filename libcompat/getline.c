#define _GNU_SOURCE
#include <stdio.h>
#include "dietwarning.h"

#undef getline
ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
  return getdelim(lineptr,n,'\n',stream);
}
link_warning("getline","warning: you used getline without include stdio.h w/_GNU_SOURCE")
