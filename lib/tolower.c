#include <ctype.h>

inline int tolower(int c) {
  return (c>='A' && c<='Z')?c-'A'+'a':c;
}

