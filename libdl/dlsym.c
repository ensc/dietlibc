#include "_dl_int.h"

void *_dlsym(void *handle, char *symbol)
{
  unsigned long * ret=0;
  if (handle) {
    struct _dl_handle * h = (struct _dl_handle *) handle;
    int hash = elf_hash(symbol);
    unsigned long  bhash = hash%HASH_BUCKET_LEN(h->hash_tab);
    unsigned long* chain = HASH_CHAIN(h->hash_tab);
    unsigned long  ind;

//    DEBUG("dlsym: bucket(%08x,\"%s\")\n",bhash,symbol);

    ind = HASH_BUCKET(h->hash_tab)[bhash];
//    DEBUG("dlsym: chain (%08x,\"%s\")\n",ind,symbol);

    while (ind) {
      char *name = h->dyn_str_tab;
      int ptr = h->dyn_sym_tab[ind].st_name;
//      DEBUG("dlsym: symbol(\"%s\",\"%s\")\n",name+ptr,symbol);
      if (strcmp(name+ptr,symbol)==0) {
	if (h->dyn_sym_tab[ind].st_value!=0) {
	  ret = (long*)(h->mem_base+h->dyn_sym_tab[ind].st_value);
	  break;	/* ok found ... */
	}
      }
      ind = chain[ind];
    }
    DEBUG("dlsym: symbol \"%s\" @ %08lx\n",symbol,(long)ret);
  }
  return ret;
}

void *dlsym(void *handle, char *symbol) {
  _dl_error_location = "dlsym";
  _dl_error_data = symbol;
  _dl_error = 4;
  return _dlsym(handle,symbol);
}
