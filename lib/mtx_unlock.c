#define _REENTRANT
#include <threads.h>
#include <sys/futex.h>
#include <errno.h>

int mtx_unlock(mtx_t* mutex) {
  int i,r;
  thrd_t me=thrd_current();
  if (mutex->lock==0) return thrd_error;
  if ((mutex->type&mtx_recursive) && mutex->owner==me) {
    if (__sync_add_and_fetch(&mutex->lock,-1)==0) {
      // If we get here, the mutex was recursive, we unlocked it, and we
      // were the last guy holding the lock.  Wake waiters.
      mutex->owner=0;	// cosmetic; not really needed
      futex(&mutex->lock,FUTEX_WAKE,1,0,0,0);
    }
    return thrd_success;
  }
  if (__sync_val_compare_and_swap(&mutex->lock,1,0)==1)
    futex(&mutex->lock,FUTEX_WAKE,1,0,0,0);
  return thrd_success;
}
