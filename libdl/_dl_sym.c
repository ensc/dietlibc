#include "_dl_int.h"

#if 0
static void dummy(unsigned long t) {
  printf("shit happens (called from %08lx)\n",*((&t)-1));
}
#endif

void *_dl_sym_search(struct _dl_handle * h, int symbol)
{
  void *sym=0;
  struct _dl_handle * tmp;
  char *name = h->dyn_str_tab+h->dyn_sym_tab[symbol].st_name;
  DEBUG("_dl_sym_search: search for: %s\n",name);
  for (tmp=_dl_root_handle;tmp && (!sym);tmp=tmp->next) {
    DEBUG("_dl_sym_search: searching: %08lx %08lx\n",(long)tmp, (long)h);
    if (tmp==h) continue;
//    if (!tmp->flag_global) continue;
    DEBUG("_dl_sym_search: searching in %s\n",tmp->name);
    sym=_dlsym((void*)tmp,name);
    if (sym) DEBUG("_dl_sym_search: found: %s @ %08lx\n",name,(long)sym);
  }
//  if (sym) return sym;
//  return &dummy; // sym;
  return sym;
}

void *_dl_sym(struct _dl_handle * h, int symbol)
{
  void * ret=0;
  if (ELF32_ST_TYPE(h->dyn_sym_tab[symbol].st_shndx)!=0) {
    ret = h->mem_base+h->dyn_sym_tab[symbol].st_value;
  }
  else {
    ret = _dl_sym_search(h,symbol);
  }
  DEBUG("_dl_sym %d -> %08lx\n",symbol,(long)ret);
  return ret;
}
