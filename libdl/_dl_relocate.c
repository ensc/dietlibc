#include "_dl_int.h"

#if 0
/*--- are other relocation types vital to shared objects ? ---*/

  R_386_NONE		 0	/* No reloc */
  R_386_32		 1	/* Direct 32 bit  */
  R_386_COPY		 5	/* Copy symbol at runtime ?!? */
  R_386_GLOB_DAT	 6	/* Create GOT entry */
  R_386_JMP_SLOT	 7	/* Create PLT entry */
  R_386_RELATIVE	 8	/* Adjust by program base */

  R_ARM_NONE		 0	/* No reloc */
  R_ARM_ABS32		 2	/* Direct 32 bit  */
  R_ARM_COPY		20	/* Copy symbol at runtime */
  R_ARM_GLOB_DAT	21	/* Create GOT entry */
  R_ARM_JUMP_SLOT	22	/* Create PLT entry */
  R_ARM_RELATIVE	23	/* Adjust by program base */

#endif

int _dl_apply_relocate(struct _dl_handle* dh, Elf32_Rel *rel) {
  int typ,ret=0;
  Elf32_Addr *loc;

  rel = (void*)rel+(long)dh->mem_base;
  loc = (Elf32_Addr *)(dh->mem_base+rel->r_offset);

  DEBUG("_dl_apply_relocate %d @ %08lx (%08lx)\n",ELF32_R_TYPE(rel->r_info),(unsigned long)loc,*(unsigned long*)loc);

  typ = ELF32_R_TYPE(rel->r_info);

#ifdef __i386__
  if (typ==R_386_32) {			/* 1 */
    *loc = (unsigned long)(dh->mem_base+dh->dyn_sym_tab[ELF32_R_SYM(rel->r_info)].st_value);
  } else if (typ==R_386_COPY)  {	/* 5 */
    int len=dh->dyn_sym_tab[ELF32_R_SYM(rel->r_info)].st_size;
    DEBUG("_dl_apply_relocate: R_386_COPY !\n");
    memcpy(loc,(unsigned long)_dl_sym_search(dh, ELF32_R_SYM(rel->r_info)),len);
  } else if (typ==R_386_GLOB_DAT) {	/* 6 */
    *loc = (unsigned long)_dl_sym(dh, ELF32_R_SYM(rel->r_info));
  } else if (typ==R_386_JMP_SLOT) {	/* 7 */
    *loc += (unsigned long)dh->mem_base;
  } else if (typ==R_386_RELATIVE) {	/* 8 */
    *loc += (unsigned long)dh->mem_base;
  } else if (typ==R_386_NONE) {		/* 0 */
  } else
    ret=1;
#endif
#ifdef __arm__
  if (typ==R_ARM_ABS32) {		/*  2 */
    *loc = (unsigned long)(dh->mem_base+dh->dyn_sym_tab[ELF32_R_SYM(rel->r_info)].st_value);
  } else if (typ==R_ARM_COPY)  {	/* 20 */
    int len=dh->dyn_sym_tab[ELF32_R_SYM(rel->r_info)].st_size;
    DEBUG("_dl_apply_relocate: R_ARM_COPY !\n");
    memcpy(loc,(unsigned long)_dl_sym_search(dh, ELF32_R_SYM(rel->r_info)),len);
  } else if (typ==R_ARM_GLOB_DAT) {	/* 21 */
    *loc = (unsigned long)_dl_sym(dh, ELF32_R_SYM(rel->r_info));
  } else if (typ==R_ARM_JUMP_SLOT) {	/* 22 */
    *loc += (unsigned long)dh->mem_base;
  } else if (typ==R_ARM_RELATIVE) {	/* 23 */
    *loc += (unsigned long)dh->mem_base;
  } else if (typ==R_ARM_NONE) {		/*  0 */
  } else
    ret=1;
#endif

  DEBUG("_dl_apply_relocate @ %08lx val %08lx\n",(unsigned long)loc,*(unsigned long*)loc);
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

