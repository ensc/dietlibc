#define __USE_EXTERN_INLINES 1
#include <signal.h>

# define __sigmask(sig) \
  (((unsigned long int) 1) << (((sig) - 1) % (8 * sizeof (unsigned long int))))

# define __sigword(sig) (((sig) - 1) / (8 * sizeof (unsigned long int)))

# define _SIGSET_NWORDS	(1024 / (8 * sizeof (unsigned long int)))
typedef struct
  {
    unsigned long int __val[_SIGSET_NWORDS];
  } __sigset_t;

int __sigaddset(__sigset_t *set, int signo) {
  unsigned long int __mask = __sigmask (signo);
  unsigned long int __word = __sigword (signo);
  return ((set->__val[__word] |= __mask), 0);
}

int sigaddset (sigset_t *env, int signo) __attribute__((weak,alias("__sigaddset")));
