#include <sys/types.h>
#include <dietstdio.h>
#include <unistd.h>

extern int __stdio_atexit;
extern void __stdio_flushall(void);

extern int __stdio_parse_mode(const char *mode);
extern FILE* __stdio_init_file(int fd);

FILE *fopen (const char *path, const char *mode) {
  int f=0;	/* O_RDONLY, O_WRONLY or O_RDWR */
  int fd;

  f=__stdio_parse_mode(mode);
  if ((fd=open(path,f,0666))<0)
    return 0;
  return __stdio_init_file(fd);
}
