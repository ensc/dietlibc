#include "_dl_int.h"

int dlclose (void *handle)
{
  if (handle) {
    struct _dl_handle *h = handle;
    if (h->lnk_count) {
      --h->lnk_count;
      return -1;
    }
    if (munmap(h->mem_base,h->mem_size)!=0) return -1;
  }
  return 0;
}
