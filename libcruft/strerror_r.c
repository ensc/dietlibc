#include <string.h>

extern const char __sys_err_unknown[];

/* This function is _really_ useless, since strerror does not have a
 * static buffer or possibly conflicting data manipulations that could
 * be optimized away with this function.  What a crook.  Why oh why
 * doesn't susv3 define gethostbyname_r but does define this crap? */
int strerror_r(int errnum, char *buf, size_t n) {
  const char* x=strerror(errnum);
  if (x==__sys_err_unknown || n<1) return -1;
  strncpy(buf,strerror(errnum),n);
  buf[n-1]=0;
  return 0;
}
