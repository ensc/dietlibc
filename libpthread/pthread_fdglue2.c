#include "dietstdio.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

extern int __stdio_atexit;
extern FILE* __stdio_init_file_nothreads(int fd,int closeonerror);

FILE* __stdio_init_file(int fd,int closeonerror) {
  FILE *tmp=__stdio_init_file_nothreads(fd,closeonerror);
  if (tmp) pthread_mutex_init(&tmp->m,0);
  return tmp;
}
