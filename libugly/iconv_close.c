#include <iconv.h>

int __diet_iconv_close(iconv_t cd) {
  return 0;
}

int iconv_close(iconv_t cd) __attribute__((weak,alias("__diet_iconv")));
