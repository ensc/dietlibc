#ifndef ___DL_INT_H__
#define ___DL_INT_H__

#include <elf.h>
#include <dlfcn.h>

struct _dl_handle {
  struct _dl_handle *next;
  struct _dl_handle *prev;

  char *	so_name;
  int		flag_global;

  /* basic */
  void *	mem_base;	/* base address of maped *.so */
  unsigned long mem_size;	/* len of mem block */
  unsigned long lnk_count;	/* reference count (other libraries) */

  /* symbol resolve helper */
  unsigned long*hash_tab;	/* hash table */

  unsigned long*got;		/* global offset table */

  char *	dyn_str_tab;	/* dyn_name table */

  Elf32_Sym *	dyn_sym_tab;	/* dynamic symbol table */
  Elf32_Rel *	plt_rel;	/* PLT relocation table */

  /* INIT / FINI */
  void (*fini)(void);
};

#define HASH_BUCKET_LEN(p)	(*((p)))
#define HASH_BUCKET(p)		((p)+2)

#define HASH_CHAIN_LEN(p)	(*((p)+1))
#define HASH_CHAIN(p)		((p)+2+HASH_BUCKET_LEN(p))

/* elf_hash.c */
unsigned long elf_hash(const unsigned char *name);

/* _dl_alloc.c */
extern struct _dl_handle* _dl_root_handle;
extern struct _dl_handle* _dl_top_handle;
void _dl_free_handle(struct _dl_handle* dh);
struct _dl_handle* _dl_get_handle();

/* _dl_open.c */
void *_dl_open(const char*pathname, int fd, int flag);

/* _dl_search.c */
void _dl_set_rpath(const char *path);
int _dl_search(char *buf, int len, const char *filename);

/* _dl_sym.c */
void *_dl_sym(struct _dl_handle * h, int symbol);

#endif
