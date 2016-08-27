#include <pthread.h>
#include <errno.h>

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock) {
  unsigned int cur;
  do {
    cur=rwlock->n;
    if (cur&0x80000000)
      return EBUSY;
    if (cur>=0x10000000)
      return EAGAIN;
  } while (!__sync_bool_compare_and_swap(&rwlock->n,cur,cur+1));
  return 0;
}
