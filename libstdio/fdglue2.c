#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "dietstdio.h"
#include <stdlib.h>
#include <pthread.h>

extern int __stdio_atexit;

FILE* __stdio_init_file_nothreads(int fd,int closeonerror);
FILE* __stdio_init_file_nothreads(int fd,int closeonerror) {
  FILE *tmp=(FILE*)malloc(sizeof(FILE));
  if (!tmp) goto err_out;
  tmp->buf=(char*)malloc(BUFSIZE);
  if (!tmp->buf) {
    free(tmp);
err_out:
    if (closeonerror) close(fd);
    errno=ENOMEM;
    return 0;
  }
  tmp->fd=fd;
  tmp->bm=0;
  tmp->bs=0;
  tmp->buflen=BUFSIZE;
  tmp->flags=0;
  if (__stdio_atexit==0) {
    __stdio_atexit=1;
    atexit(__stdio_flushall);
  }
  tmp->next=__stdio_root;
  __stdio_root=tmp;
  tmp->ungotten=0;
  return tmp;
}

FILE* __stdio_init_file(int fd,int closeonerror) __attribute__((weak,alias("__stdio_init_file_nothreads")));
