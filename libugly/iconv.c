#include <errno.h>
#include <iconv.h>
#include "dietfeatures.h"

size_t __diet_iconv(iconv_t cd, const char* * inbuf, size_t *
		    inbytesleft, char* * outbuf, size_t * outbytesleft) {
  errno=EINVAL;
  return (size_t)(-1);
}

size_t iconv(iconv_t cd, const char** inbuf, size_t* inbytesleft, char** outbuf,
	      size_t *outbytesleft)  __attribute__((weak,alias("__diet_iconv")));
