#include <threads.h>
#include <sys/futex.h>

int cnd_broadcast(cnd_t* cond) {
  cond->sem=1;
  futex(&cond->sem,FUTEX_WAKE,0x7fffffff,0,0,0);
  return thrd_success;
}
