#define _REENTRANT
#include <threads.h>
#include <sys/futex.h>
#include <sys/mman.h>
#include <sys/tls.h>
#include <errno.h>

int thrd_join(thrd_t thr, int* res) {
  int flags=thr->flags;
  int r;
  while ((flags&6) == 4) {
    /* if flags&4 then thrd_exit is already working on that thread.
     * when it's done, it does flags|=2.
     * so we wait for that. */
    __sync_synchronize();
    flags=thr->flags;
  }
  if ((flags&2)==0) {	/* is the thread still running? */
    if (flags&5)
      // thread is detached or in the process of being cleaned up, refuse join
      return thrd_error;
    if (!__sync_bool_compare_and_swap(&thr->join_futex,0,1))
      // somebody is already calling thrd_join, refuse join
      return thrd_error;
    do {
      r=futex(&thr->join_futex,FUTEX_WAIT,1,0,0,0);
    } while (r==-1 && errno==EINTR);
    if (res) *res=thr->res;
    thr->join_wait_futex=1;
    futex(&thr->join_wait_futex,FUTEX_WAKE,1,0,0,0);
  } else {
    /* thread is already dead, do cleanup */
    if (res) *res=thr->res;
    munmap(thr->arg,thr->memsize);
  }
  return thrd_success;
}

