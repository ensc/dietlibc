#include <locale.h>

char *setlocale (int category, const char *locale) {
  (void)category;
  (void)locale;
  return 0;
}
