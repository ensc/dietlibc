#include <threads.h>
#include <sys/futex.h>

int cnd_signal(cnd_t* cond) {
  cond->sem=1;
  futex(&cond->sem,FUTEX_WAKE,1,0,0,0);
  return thrd_success;
}
