#include "_dl_int.h"

static void dummy(unsigned long t) {
  printf("shit happens (called from %08x)\n",*((&t)-1));
}

void *_dl_sym_search(struct _dl_handle * h, int symbol)
{
  char *name = h->dyn_str_tab+h->dyn_sym_tab[symbol].st_name;
  printf("search for: %s\n",name);
  
  return &dummy;
}

void *_dl_sym(struct _dl_handle * h, int symbol)
{
  void * ret=0;
  if (ELF32_ST_TYPE(h->dyn_sym_tab[symbol].st_info)!=0) {
    ret = h->mem_base+h->dyn_sym_tab[symbol].st_value;
  }
  else {
    ret = _dl_sym_search(h,symbol);
  }
  printf("_dl_sym %d -> %08x\n",symbol,ret);
  return ret;
}
