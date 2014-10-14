#define _GNU_SOURCE
#include <search.h>
#include <stdlib.h>

static void freelist(struct bucket* b) {
  while (b) {
    struct bucket* o=b;
    b=b->next;
    free(o);
  }
}

void hdestroy_r(struct hsearch_data* htab) {
  size_t i;
  for (i=0; i<htab->size; ++i) {
    freelist(htab->table[i]);
    htab->table[i]=0;
  }
}
