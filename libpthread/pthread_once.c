#include <pthread.h>

int testandset(int*);
int __pthread_once (pthread_once_t* once_control, void (*init_routine)(void))
{
  if (!testandset(once_control)) init_routine();
  return 0;
}
int pthread_once (pthread_once_t* once_control, void (*init_routine)(void)) __attribute__((alias("__pthread_once")));
