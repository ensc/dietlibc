#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "parselib.h"

/* open and mmap file, fill in struct state, return 0.
 * return -1 on failure */
void __end_parse(struct state* s) {
  munmap((void*)(s->buffirst),s->buflen);
  s->buffirst=0;
}
