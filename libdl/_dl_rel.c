#include <dlfcn.h>

#include "_dl_int.h"

static void exit_now(void) {
  DEBUG("exit_now: symbol not found\n");
  _exit(213);
}

unsigned long do_rel(struct _dl_handle * tmp_dl, unsigned long off)
{
  Elf32_Rel *tmp = ((void*)tmp_dl->plt_rel)+off;

  int sym=ELF32_R_SYM(tmp->r_info);

  register unsigned long sym_val;

  DEBUG("do_rel %08lx %08lx\n",(long)tmp_dl,off);
  DEBUG("do_rel %08lx+%lx\n",(long)tmp_dl->plt_rel,off);
  DEBUG("do_rel @ %08lx with type %d -> %d\n",(long)tmp->r_offset,ELF32_R_TYPE(tmp->r_info),sym);
  DEBUG("do_rel sym @ %08lx\n",(long)tmp_dl->dyn_sym_tab);
  DEBUG("do_rel sym %08lx\n",(long)tmp_dl->dyn_sym_tab[sym].st_value);

  /* modify GOT for REAL symbol */
  //sym_val=((unsigned long)(tmp_dl->mem_base+tmp_dl->dyn_sym_tab[sym].st_value));
  sym_val=(unsigned long)_dl_sym(tmp_dl,sym);
  *((unsigned long*)(tmp_dl->mem_base+tmp->r_offset))=sym_val;

  DEBUG("do_rel sym %08lx\n",(long)sym_val);
  /* JUMP (arg sysdep...) */
  if (sym_val) return sym_val;
  /* can't find symbol -> die now */
  return (unsigned long)exit_now;
}


