#include <threads.h>

int tss_set(tss_t tss_id, void* val) {
  tss_id->data=val;
  return thrd_success;
}
