#include <stdio.h>

void setlinebuf(FILE* stream) {
  setvbuf(stream,0,_IOLBF,BUFSIZ);
}
