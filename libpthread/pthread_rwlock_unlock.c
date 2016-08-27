#include <pthread.h>
#include <errno.h>

int pthread_rwlock_unlock(pthread_rwlock_t *rwlock) {
  unsigned int cur,next;
  do {
    cur=rwlock->n;
    if (cur==0x80000000)
      next=0;
    else if (cur>0)
      next=cur-1;
    else {
      errno=EPERM;
      return -1;
    }
  } while (!__sync_bool_compare_and_swap(&rwlock->n,cur,next));
  return 0;
}
