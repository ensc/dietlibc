#include <sys/types.h>
#include <dietstdio.h>
#include <unistd.h>

extern int __stdio_atexit;

FILE *fopen (const char *path, const char *mode) {
  int f=0;	/* O_RDONLY, O_WRONLY or O_RDWR */
  int fd;

  f=__stdio_parse_mode(mode);
  if ((fd=open(path,f,0666))<0)
    return 0;
  return __stdio_init_file(fd,1);
}
