#include <threads.h>

extern thread_local tss_t tss_dtor_list;

void tss_delete(tss_t tss_id) {
  tss_t* cur;
  for (cur=&tss_dtor_list; *cur; cur=&((*cur)->next)) {
    if (*cur==tss_id)
      *cur=tss_id->next;
  }
  tss_id->dtor=0;
  tss_id->data=0;
  tss_id->next=0;
}
