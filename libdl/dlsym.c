#include "_dl_int.h"

#include "elf_hash.h"

#ifdef __DIET_LD_SO__
static
#endif
void *_dlsym(void*handle,char*symbol) {
  unsigned long*sym=0;
  if (handle) {
    struct _dl_handle*dh=(struct _dl_handle*)handle;
    unsigned long hash =elf_hash(symbol);
    unsigned long bhash=hash%HASH_BUCKET_LEN(dh->hash_tab);
    unsigned long*chain=HASH_CHAIN(dh->hash_tab);
    unsigned long ind;
    char *name=dh->dyn_str_tab;

//    DEBUG("dlsym: bucket(%08x,\"%s\")\n",bhash,symbol);

    ind=HASH_BUCKET(dh->hash_tab)[bhash];
//    DEBUG("dlsym: chain (%08x,\"%s\")\n",ind,symbol);

    while(ind) {
      int ptr=dh->dyn_sym_tab[ind].st_name;
//      DEBUG("dlsym: symbol(\"%s\",\"%s\")\n",name+ptr,symbol);
      if (strcmp(name+ptr,symbol)==0) {
	if (dh->dyn_sym_tab[ind].st_value!=0) {
	  sym=(long*)(dh->mem_base+dh->dyn_sym_tab[ind].st_value);
	  break;	/* ok found ... */
	}
      }
      ind=chain[ind];
    }
    DEBUG("dlsym: symbol \"%s\" @ %08lx\n",symbol,(long)sym);
  }
  return sym;
}

void *dlsym(void*handle,char*symbol) {
  _dl_error_location="dlsym";
  _dl_error_data=symbol;
  _dl_error=4;
  return _dlsym(handle,symbol);
}
