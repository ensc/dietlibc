#include <pthread.h>

int pthread_rwlock_init(pthread_rwlock_t *rwlock, const pthread_rwlockattr_t *attr) {
  (void)attr;
  rwlock->n=0;
  return 0;
}
