#include <sys/types.h>
#include <dietstdio.h>
#include <unistd.h>

#define SEEK_END 2

extern int __stdio_atexit;
extern void __stdio_flushall();

extern int __stdio_parse_mode(const char *mode,int *seektoend);
extern FILE* __stdio_init_file(int fd);

FILE *fopen (const char *path, const char *mode) {
  int f=0;	/* O_RDONLY, O_WRONLY or O_RDWR */
  int seektoend=0;
  int fd;

  f=__stdio_parse_mode(mode,&seektoend);
  if ((fd=open(path,f,0666))<0)
    return 0;
  if (seektoend) lseek(fd,0,SEEK_END);
  return __stdio_init_file(fd);
}
