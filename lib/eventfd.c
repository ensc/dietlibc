#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/eventfd.h>

extern int __eventfd(unsigned int count);
extern int __eventfd2(unsigned int count,int flags);

int eventfd(unsigned int count,int flags) {
  int r=__eventfd2(count,flags);
  if (r==-1 && errno==ENOSYS) {
    r=__eventfd(count);
    if (r!=-1 && flags) {
      int x;
      x=fcntl(r,F_SETFD,flags);
      if (x==-1)
	close(r);
    }
  }
  return r;
}
