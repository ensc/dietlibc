#include <unistd.h>
#include <string.h>
#include "dietstdio.h"
#include "dietfeatures.h"

int puts(const char *s) {
#ifdef WANT_BUFFERED_STDIO
  return fwrite(s,1,strlen(s),stdout) && fputc('\n',stdout);
#else
  return write(1,s,strlen(s)) && write(1,"\n",1);
#endif
}
