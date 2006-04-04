#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include "dietlocale.h"

char *setlocale (int category, const char *locale) {
  if (locale && (category==LC_ALL || category==LC_CTYPE)) {
    if (!*locale) {
      char* x;
      lc_ctype=CT_8BIT;
      x=getenv("LC_CTYPE");
      if (!x) x=getenv("LC_ALL");
      if (!x) x="C";
      if (strstr(x,".UTF-8") || strstr(x,".UTF8")) lc_ctype=CT_UTF8;
    }
  }
  if (locale && (locale[0]!='C' || locale[1])) return 0;
  return "C";
}
