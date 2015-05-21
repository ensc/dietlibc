#define _REENTRANT
#define _DIETLIBC_SOURCE
#include <threads.h>
#include <sys/futex.h>
#include <errno.h>

int mtx_timedlock(mtx_t* mutex, const struct timespec* time_point) {
  int i,r;
  do {
    r=__mtx_trylock(mutex,&i);
    if (r!=thrd_busy) return r;
    for (;;) {
      r=futex(&mutex->lock,FUTEX_WAIT,i,time_point,0,0);
      if (r==-1) {
	if (errno==EWOULDBLOCK) { r=0; break; } else
	if (errno==ETIMEDOUT) return thrd_timedout; else
	if (errno==EINTR) continue;
      } else
	break;
    }
  } while (r==0);
  return thrd_error;
}
