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
#ifdef DEBUG
  pf(__func__": search for: "); pf(name); pf("\n");
#endif
  for (tmp=_dl_root_handle;tmp && (!sym);tmp=tmp->next) {
    if (tmp==dh) continue;
//    if (!(tmp->flags&RTLD_GLOBAL)) continue;
#ifdef DEBUG
    pf(__func__": searching in "); pf(tmp->name); pf("\n");
#endif
    sym=_dlsym((void*)tmp,name);
#ifdef DEBUG
    if (sym) { pf(__func__": found: "); pf(name); pf(" @ "); ph((long)sym); pf("\n"); }
#endif
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
#ifdef DEBUG
  pf(__func__": "); ph(symbol); pf(" -> "); ph((long)sym); pf("\n");
#endif
  return sym;
}
