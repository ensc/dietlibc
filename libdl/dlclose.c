#include <sys/mman.h>

#include "_dl_int.h"

int dlclose (void *handle)
{
  if (handle) {
    struct _dl_handle *h = handle;
    if (h->lnk_count) {
      --h->lnk_count;
      return -1;
    }
    if (h->fini) h->fini();
    if (munmap(h->mem_base,h->mem_size)==-1) return -1;
//    if (munmap(handle,4096)) return -1;
    _dl_free_handle(handle);
  }
  return 0;
}
