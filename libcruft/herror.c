#include <string.h>
#include <unistd.h>

void herror(const char* s) {
  write(2,s,strlen(s));
  write(2,": DNS error.\n",13);
}
