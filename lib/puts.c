#include <unistd.h>
#include <string.h>

int puts(const char *s) {
  return write(1,s,strlen(s)) && write(1,"\n",1);
}
