#include <pthread.h>
#include <errno.h>

int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock) {
  do {
    unsigned int cur=rwlock->n;
    if (cur)
      return EBUSY;
  } while (!__sync_bool_compare_and_swap(&rwlock->n,0,0x80000000));
  return 0;
}
