#include <stdio.h>

#include "_dl_int.h"

#if 0
  R_386_NONE:		// 0
  R_386_32:		// 1
  R_386_GLOB_DAT:	// 6
  R_386_JMP_SLOT:	// 7
  R_386_RELATIVE:	// 8

--- are this relocation types vital to shared objects ? ---
  R_386_PLT32:
  R_386_PC32:
  R_386_GOTPC:
  R_386_GOT32:
  R_386_GOTOFF:
#endif

int _dl_apply_relocate(struct _dl_handle* dh, Elf32_Rel *rel) {
  int typ,ret=0;
  Elf32_Addr *loc;

  rel = (void*)rel+(long)dh->mem_base;
  loc = (Elf32_Addr *)(dh->mem_base+rel->r_offset);

  DEBUG(printf("_dl_apply_relocate %d @ %08lx\n",ELF32_R_TYPE(rel->r_info),(unsigned long)loc);)

  typ = ELF32_R_TYPE(rel->r_info);

  if (typ==R_386_32) {		/* 1 */
    *loc += (unsigned long)rel->r_offset;	/* FIX ME */
  } else if ((typ==R_386_GLOB_DAT)||(typ==R_386_JMP_SLOT)) {	/* 6 / 7 */
    *loc = (unsigned long)_dl_sym(dh, ELF32_R_SYM(rel->r_info));
  } else if (typ==R_386_RELATIVE) {	/* 8 */
    *loc += (unsigned long)dh->mem_base;
  } else if (typ==R_386_NONE) {		/* 0 */
  } else
    ret=1;

  return ret;
}

int _dl_relocate(struct _dl_handle* dh, Elf32_Rel *rel, int num) {
  int i;
  for (i=0;i<num;i++) {
    if (_dl_apply_relocate(dh,rel+i)) {
      _dl_error=3;
      return 1;
    }
  }
  return 0;
}

