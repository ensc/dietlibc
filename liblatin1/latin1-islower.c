#include <ctype.h>

int islower(int c) {
  return (c>='a' && c<='z') || (c>=223 && c<=255 && c!=247);
}
