#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

FILE* __stdio_init_file(int fd);

FILE *tmpfile (void) {
  int fd;
  char template[20] = "/tmp/tmpfile-XXXXXX";
  if ((fd=mkstemp(template))<0)
    return 0;
  unlink(template);
  return __stdio_init_file(fd);
}
