#include <sys/stat.h>

extern void __stat_convert(void *d,struct stat *s);

#undef fstat
int fstat(int fd,void *d) {
  struct stat s;
  int res=__dietfstat(fd,&s);
  if (res==0) __stat_convert(d,&s);
  return res;
}
