#include <stdio.h>
#undef setlinebuf

void setlinebuf(FILE* stream) {
  setvbuf(stream,0,_IOLBF,BUFSIZ);
}
