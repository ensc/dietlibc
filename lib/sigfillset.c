#define __USE_EXTERN_INLINES 1
#include <signal.h>

# define _SIGSET_NWORDS	(1024 / (8 * sizeof (unsigned long int)))
typedef struct
  {
    unsigned long int __val[_SIGSET_NWORDS];
  } __sigset_t;

int __sigfillset (__sigset_t *set) {
  int i;
  for (i=0; i<_SIGSET_NWORDS; i++)
    set->__val[i]=~0;
  return 0;
}

int sigfillset(sigset_t *env, int signo) __attribute__((weak,alias("__sigfillset")));
