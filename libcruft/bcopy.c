#include <sys/types.h>
#include "dietwarning.h"

extern void memmove(char*,const char*,size_t);

void bcopy(const void *src, void *dest, size_t n);
void bcopy(const void *src, void *dest, size_t n) {
  memmove(dest,src,n);
}

link_warning("bcopy","warning: you used bcopy without including dietlibc <string.h> w/ _BSD_SOURCE!")
