#include <sys/types.h>
#include "parselib.h"

size_t __parse_nws(struct state* s) {
  return __parse(s,isnonblank);
}
