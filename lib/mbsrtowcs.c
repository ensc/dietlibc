#include <wchar.h>

size_t mbsrtowcs(wchar_t *dest, const char **src, size_t len, mbstate_t *ps) {
  const char* orig=*src;
  size_t i;
  for (i=0; i<len; ++i) {
    size_t n=mbrtowc(dest?dest+i:0,*src,len,ps);
    if (n==(size_t)-1) return -1;
    if (!n) break;
    *src+=n;
  }
  return *src-orig;
}
