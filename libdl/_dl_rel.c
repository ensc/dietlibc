#include <stdio.h>
#include <dlfcn.h>
#include <linux/elf.h>

#include "_dl_int.h"

static void exit_now(void) {
#ifdef DEBUG
  printf("exit_now: symbol not found\n");
#endif
  _exit(213);
}

unsigned long do_rel(struct _dl_handle * tmp_dl, unsigned long off)
{
  Elf32_Rel *tmp = ((void*)tmp_dl->plt_rel)+off;

  int sym=ELF32_R_SYM(tmp->r_info);

  register unsigned long sym_val;

#ifdef DEBUG
  printf("do_rel %08lx %08lx\n",(long)tmp_dl,off);
  printf("do_rel %08lx+%lx\n",(long)tmp_dl->plt_rel,off);
  printf("do_rel @ %08lx with type %d -> %d\n",(long)tmp->r_offset,ELF32_R_TYPE(tmp->r_info),sym);
  printf("do_rel sym %08lx\n",(long)tmp_dl->dyn_sym_tab[sym].st_value);
#endif

  /* modify GOT for REAL symbol */
  //sym_val=((unsigned long)(tmp_dl->mem_base+tmp_dl->dyn_sym_tab[sym].st_value));
  sym_val=(unsigned long)_dl_sym(tmp_dl,sym);
  *((unsigned long*)(tmp_dl->mem_base+tmp->r_offset))=sym_val;

#ifdef DEBUG
  printf("do_rel sym %08lx\n",(long)sym_val);
#endif
  /* JUMP (arg sysdep...) */
  if (sym_val) return sym_val;
  return (unsigned long)exit_now;
}


