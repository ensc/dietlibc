#include "dietstdio.h"
#include <stdlib.h>
#include <unistd.h>

FILE *tmpfile (void) {
  int fd;
  char template[20] = "/tmp/tmpfile-XXXXXX";
  if ((fd=mkstemp(template))<0)
    return 0;
  unlink(template);
  return __stdio_init_file(fd,1);
}
