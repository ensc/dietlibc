#include "_dl_int.h"
#include <stdio.h>

static void dummy(unsigned long t) {
  printf("shit happens (called from %08lx)\n",*((&t)-1));
}

static void *_dl_sym_search(struct _dl_handle * h, int symbol)
{
  void *sym;
  struct _dl_handle * tmp;
  char *name = h->dyn_str_tab+h->dyn_sym_tab[symbol].st_name;
  printf("search for: %s\n",name);
  for (tmp=_dl_root_handle;tmp && (!sym);tmp=tmp->next) {
    if (tmp==h) continue;
    printf("search for: %s(%s)\n",name,tmp->so_name);
    sym=dlsym((void*)tmp,name);
  }
  return &dummy; // sym;
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
  printf("_dl_sym %d -> %08lx\n",symbol,(long)ret);
  return ret;
}
