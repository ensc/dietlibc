#include <string.h>
#include "dietfeatures.h"
#include <errno.h>
#include <stdlib.h>
#include "dieticonv.h"

static enum charset parsecharset(const char* s) {
  if (!strcasecmp(s,"UTF-8")) return UTF_8; else
  if (!strcasecmp(s,"UCS-4")) return UCS_4; else
  if (!strcasecmp(s,"ISO-8859-1")) return ISO_8859_1; else
  if (!strcasecmp(s,"US-ASCII")) return ISO_8859_1; else
  return INVALID;
}

iconv_t __diet_iconv_open(const char* tocode, const char* fromcode) {
  iconv_t foo=(iconv_t)malloc(sizeof(struct dieticonv));
  if (!foo) {
    errno=ENOMEM;
    return (iconv_t)(-1);
  }
  foo->from=parsecharset(fromcode);
  foo->to=parsecharset(tocode);
  if (foo->from==INVALID || foo->to==INVALID) {
    free(foo);
    errno=EINVAL;
    return (iconv_t)(-1);
  }
  return foo;
}

iconv_t iconv_open(const char* tocode,const char* fromcode)  __attribute__((weak,alias("__diet_iconv_open")));
