#include "dietstdio.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>

#ifdef WANT_BUFFERED_STDIO
extern int __stdio_atexit;
extern void __stdio_flushall();
#endif

FILE* __stdio_init_file(int fd) {
  FILE *tmp=(FILE*)malloc(sizeof(FILE));
  if (!tmp) {
    close(fd);
#ifdef WANT_THREAD_SAFE
    *(__errno_location())=ENOMEM;
#else
    errno=ENOMEM;
#endif
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
  pthread_mutex_init(&tmp->m,0);
  return tmp;
}
