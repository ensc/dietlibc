#include <pthread.h>
#include <assert.h>
#include <errno.h>

int main() {
  pthread_rwlock_t r=PTHREAD_RWLOCK_INITIALIZER;
  int i;

  assert(pthread_rwlock_tryrdlock(&r)==0);
  assert(pthread_rwlock_unlock(&r)==0);

  assert(pthread_rwlock_tryrdlock(&r)==0);
  assert(pthread_rwlock_tryrdlock(&r)==0);
  i=pthread_rwlock_trywrlock(&r);
  assert(pthread_rwlock_trywrlock(&r)==EBUSY);
  assert(pthread_rwlock_unlock(&r)==0);
  assert(pthread_rwlock_unlock(&r)==0);

  assert(pthread_rwlock_trywrlock(&r)==0);
  assert(pthread_rwlock_tryrdlock(&r)==EBUSY);
  assert(pthread_rwlock_trywrlock(&r)==EBUSY);
}
