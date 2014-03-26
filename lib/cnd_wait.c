#include <threads.h>
#include <sys/futex.h>
#include <errno.h>

int cnd_wait(cnd_t* cond, mtx_t* mutex) {
  return cnd_timedwait(cond,mutex,0);
}
