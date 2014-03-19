#include <threads.h>

int mtx_init(mtx_t* mutex, int type) {
  mutex->lock=0;
  mutex->type=type;
  return thrd_success;
}
