#include <sys/types.h>
#include <linux/fcntl.h>

#ifndef O_LARGEFILE
#define O_LARGEFILE 0
#endif

extern int open(const char* file,int oflag,int mode);

int __libc_open64(const char* file,int oflag,int mode) {
  return open(file,oflag|O_LARGEFILE,mode);
}

int open64(const char* file,int oflag,int mode) __attribute__((weak,alias("__libc_open64")));
