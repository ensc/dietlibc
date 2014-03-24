#include <wchar.h>
#include <errno.h>

size_t mbrlen(const char *s, size_t n, mbstate_t *ps) {
  static mbstate_t internal;
  return mbrtowc (NULL, s, n, ps ?: &internal);
}
