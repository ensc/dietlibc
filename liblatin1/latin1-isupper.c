#include <ctype.h>

int isupper(int c) {
  return (c>='A' && c<='Z') || (c>=192 && c<=222 && c!=215);
}
