#ifndef ___DL_INT_H__
#define ___DL_INT_H__

#include <elf.h>
#include <dlfcn.h>

#if 0
#include <stdio.h>
#define DEBUG(x, args...)	printf(x , ## args )
#else
#define DEBUG(x, args...)
#endif

struct _dl_handle {
  struct _dl_handle *next;
  struct _dl_handle *prev;

  char *	name;		/* name of shared object */
  int		flag_global;	/* global depending names can resolve to this object */
  int		flag_system;	/* if non 0 run fini in dyn_fini */

  /* basic */
  char *	mem_base;	/* base address of maped *.so */
  unsigned long mem_size;	/* len of mem block */
  unsigned long lnk_count;	/* reference count (other libraries) */

  /* symbol resolve helper */
  unsigned long*hash_tab;	/* hash table */

  unsigned long*pltgot;		/* PLT / GOT */
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
extern struct _dl_handle* _dl_free_list;
void _dl_free_handle(struct _dl_handle* dh);
struct _dl_handle* _dl_get_handle();
struct _dl_handle* _dl_find_lib(const char* name);

/* _dl_open.c */
void *_dl_open(const char* filename, int flags);

/* _dl_load.c */
void *_dl_load(const char* filename, const char*pathname, int fd, int flags);
struct _dl_handle* _dl_dyn_scan(struct _dl_handle* dh, void* dyn_addr, int flags);

/* _dl_search.c */
void _dl_set_rpath(const char *path);
const char* _dl_get_rpath();
int _dl_search(char *buf, int len, const char *filename);

/* _dl_sym.c */
void *_dl_sym(struct _dl_handle * h, int symbol);
/* dlsym.c */
void *_dl_sym_search(struct _dl_handle * h, int symbol);
void *_dlsym(void * h, char* symbol);

/* _dl_queue.c */
int _dl_queue_lib(const char* name, int flags);
int _dl_open_dep();

/* _dl_relocate.c */
int _dl_relocate(struct _dl_handle* dh, Elf32_Rel *rel, int num);

/* dlerror.c */
extern int   _dl_error;
extern const char* _dl_error_location;
extern const char* _dl_error_data;

#endif
