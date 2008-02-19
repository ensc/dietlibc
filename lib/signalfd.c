#include <signal.h>
#include <errno.h>

int signalfd(int fd,const sigset_t* mask,int flags) {
  if (flags) {	/* bizarre glibc bullshit */
    errno=EINVAL;
    return -1;
  }
  return __signalfd(fd,mask,_NSIG/8);
}
