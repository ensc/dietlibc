#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "parselib.h"

/* open and mmap file, fill in struct state, return 0.
 * return -1 on failure */
int __prepare_parse(const char* filename,struct state* s) {
  int fd;
  s->cur=0;
  if (s->buffirst) return 0;
  fd=open(filename,O_RDONLY);
  if (fd>=0) {
    s->buflen=lseek(fd,0,SEEK_END);
    s->buffirst=mmap(0,s->buflen,PROT_READ,MAP_SHARED,fd,0);
    if (s->buffirst==(const unsigned char*)-1)
      s->buffirst=0;
    close(fd);
    return 0;
  }
  return -1;
}
