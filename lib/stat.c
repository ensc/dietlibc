#include <sys/stat.h>

extern void __stat_convert(void *d,struct stat *s);

#undef stat
int stat(const char* f,void *d) {
  struct stat s;
  int res=__dietstat(f,&s);
  if (res==0) __stat_convert(d,&s);
  return res;
}
