#include <threads.h>

int cnd_init(cnd_t* cond) {
  cond->sem=0;
  return thrd_success;
}
