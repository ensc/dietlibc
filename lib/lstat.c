#include <sys/stat.h>

extern void __stat_convert(void *d,struct stat *s);

#undef lstat
int lstat(const char *f,void *d) {
  struct stat s;
  int res=__dietlstat(f,&s);
  if (res==0) __stat_convert(d,&s);
  return res;
}
