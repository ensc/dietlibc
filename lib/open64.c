#include <linux/types.h>
#include <linux/fcntl.h>
#include <unistd.h>

#ifndef O_LARGEFILE
#define O_LARGEFILE 0
#endif
int open64(const char* file,int oflag,int mode) {
  return open(file,oflag|O_LARGEFILE,mode);
}

int __libc_open64(const char* file,int oflag,int mode) __attribute__((weak,alias("open64")));
