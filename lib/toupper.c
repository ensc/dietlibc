#include <ctype.h>

inline int toupper(int c) {
  return (c>='a' && c<='z')?c-'a'+'A':c;
}

