#include <locale.h>

char *setlocale (int category, const char *locale) {
  (void)category;
  if (locale) return 0;
  return "C";
}
