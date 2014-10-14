#include <search.h>

static struct hsearch_data hd;

int hcreate(size_t count) {
  return hcreate_r(count,&hd);
}

void hdestroy(void) {
  hdestroy_r(&hd);
}

ENTRY* hsearch(ENTRY item, ACTION action) {
  ENTRY* x;
  if (hsearch_r(item,action,&x,&hd))
    return x;
  return NULL;
}
