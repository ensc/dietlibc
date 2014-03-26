#include <stdlib.h>
#include <threads.h>

extern thread_local tss_t tss_dtor_list;

int tss_create(tss_t* tss_id, tss_dtor_t destructor) {
  *tss_id=malloc(sizeof(**tss_id));
  if (!*tss_id) return thrd_nomem;
  (*tss_id)->dtor=destructor;
  (*tss_id)->next=tss_dtor_list;
  (*tss_id)->data=0;
  tss_dtor_list=*tss_id;
  return thrd_success;
}
