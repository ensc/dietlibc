#define __USE_EXTERN_INLINES 1
#include <signal.h>
#include <errno.h>
#include <string.h>

int sigemptyset(sigset_t *set) {
  if (set==NULL) {
    __set_errno(EINVAL);
    return -1;
  }
  memset(set,0,sizeof(*set));
  return 0;
}

