#include <pthread.h>

int pthread_rwlock_destroy(pthread_rwlock_t *rwlock) {
  (void)rwlock;
  return 0;
}
