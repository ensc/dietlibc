#include "dietstdio.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#ifdef WANT_BUFFERED_STDIO
extern int __stdio_atexit;
extern void __stdio_flushall();
#endif

int __stdio_parse_mode(const char *mode,int *seektoend) {
  int f=0;
  for (;;) {
    switch (*mode) {
    case 0: return f;
    case 'b': break;
    case 'r': f=O_RDONLY; break;
    case 'w': f=O_WRONLY|O_CREAT|O_TRUNC; break;
    case 'a': f=O_WRONLY|O_CREAT; *seektoend=1; break;
    case '+': f|=O_RDWR; break;
    }
    ++mode;
  }
}

FILE* __stdio_init_file(int fd) {
  FILE *tmp=(FILE*)malloc(sizeof(FILE));
  if (!tmp) {
    close(fd);
    errno=ENOMEM;
    return 0;
  }
  tmp->fd=fd;
#ifdef WANT_BUFFERED_STDIO
  tmp->bm=0;
  tmp->bs=0;
#endif
  tmp->flags=0;
#ifdef WANT_BUFFERED_STDIO
  if (__stdio_atexit==0) {
    __stdio_atexit=1;
    atexit(__stdio_flushall);
  }
  tmp->next=__stdio_root;
  __stdio_root=tmp;
#endif
  return tmp;
}
