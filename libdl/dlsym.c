#include <stdio.h>

#include "_dl_int.h"

void *dlsym(void *handle, char *symbol)
{
  unsigned long * ret=0;
  if (handle) {
    struct _dl_handle * h = (struct _dl_handle *) handle;
    int hash = elf_hash(symbol);
    unsigned long  bhash = hash%HASH_BUCKET_LEN(h->hash_tab);
    unsigned long* chain = HASH_CHAIN(h->hash_tab);
    unsigned long  ind;

//    printf("bucket(%08x,\"%s\")\n",bhash,symbol);

    ind = HASH_BUCKET(h->hash_tab)[bhash];
//    printf("chain (%08x,\"%s\")\n",ind,symbol);

    while (ind) {
      char *name = h->dyn_str_tab;
      int ptr = h->dyn_sym_tab[ind].st_name;
//      printf("symbol(\"%s\",\"%s\")\n",name+ptr,symbol);
      if (strcmp(name+ptr,symbol)==0) {
	ret = h->mem_base+h->dyn_sym_tab[ind].st_value;
	break;	/* ok found ... */
      }
      ind = chain[ind];
    }
    printf("symbol \"%s\" @ %08lx\n",symbol,(long)ret);

  }
  return ret;
}
