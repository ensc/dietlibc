#include <sys/types.h>
#include <ctype.h>
#include "parselib.h"

size_t __parse_ws(struct state* s) {
  return __parse(s,isblank);
}
