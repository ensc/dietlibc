#ifndef ___DL_INT_H__
#define ___DL_INT_H__

#include <elf.h>

struct _dl_handle {
  /* basic */
  void *	mem_base;	/* base address of maped *.so */
  unsigned long mem_size;	/* len of mem block */
  unsigned int  lnk_count;	/* reference count */

  /* symbol resolve helper */
  unsigned long*hash_tab;	/* hash table */
  Elf32_Sym *	dyn_sym_tab;	/* dynamic symbol table */
  char *	dyn_str_tab;	/* dyn_name table */

  Elf32_Rel *	plt_rel;	/* PLT relocation table */

  /* FINI */
  void (*fini)(void);
};

#define HASH_BUCKET_LEN(p)	((p))
#define HASH_BUCKET(p)		((p)+2)

#define HASH_CHAIN_LEN(p)	((p)+1)
#define HASH_CHAIN(p)		((p)+2+HASH_BUCKET_LEN(p))

unsigned long elf_hash(const unsigned char *name);

#endif
