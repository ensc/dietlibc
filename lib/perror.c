#include "dietfeatures.h"
#include <unistd.h>
#include <string.h>

#define _BSD_SOURCE
#include <errno.h>

void perror(const char *s) {
  register const char *message="[unknown error]";
  write(2,s,strlen(s));
  write(2,": ",2);
  if (errno>=0 && errno<sys_nerr)
    message=sys_errlist[errno];
  write(2,message,strlen(message));
  write(2,"\n",1);
}
