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
  char *name=dh->dyn_str_tab+dh->dyn_sym_tab[symbol].st_name;
  return _dl_sym_search_str(dh,name);
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
  pf(__FUNCTION__); pf(": "); ph(symbol); pf(" -> "); ph((long)sym); pf("\n");
#endif
  return sym;
}
