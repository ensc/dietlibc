#include <sys/mman.h>

#include "_dl_int.h"

static void dec_referenced_libs(struct _dl_handle* dh)
{
  Elf32_Dyn* dyn_tab = (void*)(dh->got[0]);	/* for i386/arm this is the dynamic section ptr */
  int i;
  for(i=0;dyn_tab[i].d_tag;i++) {
    if (dyn_tab[i].d_tag==DT_NEEDED) {
      char *lib_name=dh->dyn_str_tab+dyn_tab[i].d_un.d_val;
      DEBUG("dlclose lib: %s\n",lib_name);
      dlclose(_dl_find_lib(lib_name));
    }
  }
}

int dlclose (void *handle)
{
  if (handle) {
    struct _dl_handle *h = handle;
    if (--(h->lnk_count)) return 0;	/* not yet unreferenced */

    DEBUG("dlclose: %s\n",h->name);
    if (h->fini) h->fini();
    dec_referenced_libs(h);
    if (munmap(h->mem_base,h->mem_size)==-1) return -1;
    _dl_free_handle(handle);
  }
  return 0;
}
