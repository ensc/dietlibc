#define _REENTRANT
#define _DIETLIBC_SOURCE
#include <threads.h>
#include <sys/futex.h>
#include <errno.h>

int __mtx_trylock(mtx_t* mutex,int* lockval) {
  int i;
  thrd_t me=thrd_current();
  /* attempt to lock the mutex with an atomic instruction */
  if ((i=__sync_val_compare_and_swap(&mutex->lock,0,1))==0) {
    mutex->owner=me;
    return thrd_success;
  }
  if ((mutex->type&mtx_recursive) && mutex->owner==me) {
    if (__sync_add_and_fetch(&mutex->lock,1)>1000) {
      __sync_add_and_fetch(&mutex->lock,-1);
      return thrd_error;
    }
    return thrd_success;
  }
  if (lockval) *lockval=i;
  return thrd_busy;
}

int mtx_trylock(mtx_t* mutex) {
  return __mtx_trylock(mutex,NULL);
}
