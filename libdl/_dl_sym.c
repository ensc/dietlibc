#include "_dl_int.h"

#if 0
static void dummy(unsigned long t) {
  printf("shit happens (called from %08lx)\n",*((&t)-1));
}
#endif

#ifdef __DIET_LD_SO__
static
#endif
void*_dl_sym_search(struct _dl_handle*dh,int symbol) {
  void *sym=0;
  struct _dl_handle*tmp;
  char *name=dh->dyn_str_tab+dh->dyn_sym_tab[symbol].st_name;
  DEBUG("_dl_sym_search: search for: %s\n",name);
  for (tmp=_dl_root_handle;tmp && (!sym);tmp=tmp->next) {
    if (tmp==dh) continue;
//    if (!tmp->flag_global) continue;
    DEBUG("_dl_sym_search: searching in %s\n",tmp->name);
    sym=_dlsym((void*)tmp,name);
    if (sym) DEBUG("_dl_sym_search: found: %s @ %08lx\n",name,(long)sym);
  }
//  if (sym) return sym;
//  return &dummy; // sym;
  return sym;
}

#ifdef __DIET_LD_SO__
static
#endif
void*_dl_sym(struct _dl_handle*dh,int symbol) {
  void*sym=0;
  if (ELF_ST_TYPE(dh->dyn_sym_tab[symbol].st_shndx)!=0) {
    sym=dh->mem_base+dh->dyn_sym_tab[symbol].st_value;
  }
  else {
    sym=_dl_sym_search(dh,symbol);
  }
  DEBUG("_dl_sym %d -> %08lx\n",symbol,(long)sym);
  return sym;
}
