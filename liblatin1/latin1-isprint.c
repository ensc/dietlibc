#include <ctype.h>

int isprint(int c) {
  return (c>=32 && c<=126) || (c>=160 && c<=255);
}
