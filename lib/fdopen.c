#include <sys/types.h>
#include <dietstdio.h>
#include <unistd.h>

#define SEEK_END 2

int __stdio_parse_mode(const char *mode,int *seektoend);
FILE* __stdio_init_file(int fd);

FILE *fdopen (int filedes, const char *mode) {
  int f=0;	/* O_RDONLY, O_WRONLY or O_RDWR */
  int seektoend=0;
  int fd;

  f=__stdio_parse_mode(mode,&seektoend);
  if ((fd=filedes)<0) return 0;
  if (seektoend) lseek(fd,0,SEEK_END);
  return __stdio_init_file(fd);
}
