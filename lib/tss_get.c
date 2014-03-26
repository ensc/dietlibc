#include <threads.h>

void* tss_get(tss_t tss_id) {
  return tss_id->data;
}
