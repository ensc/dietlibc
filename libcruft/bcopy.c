#include "dietwarning.h"

void bcopy(const void *src, void *dest, size_t n) {
  memmove(dest,src,n);
}

link_warning(bcopy,"warning: you used bcopy without including <string.h>!")
