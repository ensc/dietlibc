#include <stdlib.h>
#include "dieticonv.h"

int iconv_close(iconv_t cd) {
  free(cd);
  return 0;
}
