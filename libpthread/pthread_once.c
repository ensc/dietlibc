#include <pthread.h>
#include "thread_internal.h"

int __pthread_once (pthread_once_t* once_control, void (*init_routine)(void))
{
  if (!(__testandset(once_control))) init_routine();
  return 0;
}
int pthread_once (pthread_once_t* once_control, void (*init_routine)(void)) __attribute__((alias("__pthread_once")));
