#include <ctype.h>

int isgraph(int c) {
  return (c>=33 && c<=126) || (c>=161 && c<=255);
}

