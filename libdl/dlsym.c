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

#ifdef DEBUG
//    pf(__func__:" bucket("); ph(bhash); pf(",\""); pf(symbol); pf("\")\n");
#endif

    ind=HASH_BUCKET(dh->hash_tab)[bhash];
#ifdef DEBUG
//    pf(__func__:" chain ("); ph(ind); pf(",\""); pf(symbol); pf("\")\n");
#endif

    while(ind) {
      int ptr=dh->dyn_sym_tab[ind].st_name;
#ifdef DEBUG
//      pf(__func__:" symbol(\""); pf(name+ptr); pf("\",\"); pf(symbol); pf("\")\n");
#endif
      if (strcmp(name+ptr,symbol)==0) {
	if (dh->dyn_sym_tab[ind].st_value!=0) {
	  sym=(long*)(dh->mem_base+dh->dyn_sym_tab[ind].st_value);
	  break;	/* ok found ... */
	}
      }
      ind=chain[ind];
    }
#ifdef DEBUG
    pf(__func__": symbol \""); pf(symbol); pf("\" @ "); ph((long)sym); pf("\n");
#endif
  }
  return sym;
}

void*dlsym(void*handle,char*symbol) {
  void*h;
  if ((h=_dlsym(handle,symbol))==0) {
    _dl_error_location="dlsym";
    _dl_error_data=symbol;
    _dl_error=5;
  }
  return h;
}
