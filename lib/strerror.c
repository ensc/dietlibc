#include <string.h>
#include <errno.h>

const char *strerror(int errnum) {
  if (errnum>=0 && errnum<sys_nerr)
    return sys_errlist[errnum];
  return "[unknown error]";
}
