#include <string.h>

#define _BSD_SOURCE
#include <errno.h>

const char *strerror(int errnum) {
  if (errnum>=0 && errnum<sys_nerr)
    return sys_errlist[errnum];
  return "[unknown error]";
}
