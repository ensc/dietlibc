#include <pthread.h>

int pthread_condattr_init(pthread_condattr_t *attr)
{
  return 0;
}

int pthread_condattr_destroy(pthread_condattr_t *attr)
__attribute__((alias("pthread_condattr_init")));

int pthread_condattr_getpshared(const pthread_condattr_t *attr, int *pshared)
__attribute__((alias("pthread_condattr_init")));

int pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared)
__attribute__((alias("pthread_condattr_init")));
