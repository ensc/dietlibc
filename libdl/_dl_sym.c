#include "_dl_int.h"

static void dummy(unsigned long t) {
  printf("shit happens (called from %08x)\n",*((&t)-1));
}

void *_dl_sym(struct _dl_handle * h, int symbol)
{
  void * ret=0;
  if (ELF32_ST_TYPE(h->dyn_sym_tab[symbol].st_info)!=0) {
    ret = h->mem_base+h->dyn_sym_tab[symbol].st_value;
  }
  else {
//    _dl_sym_search();
    ret = &dummy;
  }
  printf("_dl_sym %d -> %08x\n",symbol,ret);
  return ret;
}
