#include <unistd.h>
#include <pthread.h>

pthread_t pthread_self() {
  __THREAD_INIT();

  return getpid();
}
