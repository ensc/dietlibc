#include <errno.h>
#include <iconv.h>
#include "dietfeatures.h"

iconv_t __diet_iconv_open(const char* tocode, const char* fromcode) {
  errno=EINVAL;
  return (iconv_t)(-1);
}

iconv_t iconv_open(const char* tocode,const char* fromcode)  __attribute__((weak,alias("__diet_iconv_open")));
