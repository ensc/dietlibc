#include <signal.h>

int sigfillset(sigset_t *set) {
  memset(set,0xff,sizeof(*set));
  return 0;
}
