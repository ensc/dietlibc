#include <linux/types.h>
#include <linux/fcntl.h>
#include <unistd.h>

#ifdef O_LARGEFILE
int open64(const char* file,int oflag,int mode) {
  return open(file,oflag|O_LARGEFILE,mode);
}

int __libc_open64(const char* file,int oflag,int mode) __attribute__((weak,alias("open64")));
#endif
