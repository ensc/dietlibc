#include <string.h>

extern char *sys_errlist[];
extern int sys_nerr;

char *strerror(int errnum) {
  if (errnum>=0 && errnum<sys_nerr)
    return sys_errlist[errnum];
  return "[unknown error]";
}
