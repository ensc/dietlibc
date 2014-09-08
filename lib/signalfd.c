#include <signal.h>
#include <errno.h>
#include <sys/signalfd.h>
#include <fcntl.h>

extern int __signalfd(int fd,const sigset_t* mask,size_t nsig);
extern int __signalfd4(int fd,const sigset_t* mask,size_t nsig, int flags);

int signalfd(int fd,const sigset_t* mask,int flags) {
  int r=__signalfd4(fd,mask,_NSIG/8,flags);
  if (r==-1 && errno==ENOSYS) {
    r=__signalfd(fd,mask,NSIG/8);
    if (r!=-1 && flags) {
      int x;
      x=fcntl(r,F_SETFD,flags);
      if (x==-1)
	close(r);
    }
  }
  return r;
}
