#include <ctype.h>
#include <sys/types.h>
#include "parselib.h"

int isnonblank(int ch) {
  return !isblank(ch);
}
