#include <string.h>

int main() {
  const char* test="blubber";
  if (memchr("aaaa",'x',4)) return 1;
  if (memchr(0,'x',0)) return 1;
  if (memchr(test,'u',7) != test+2) return 1;
  return 0;
}
