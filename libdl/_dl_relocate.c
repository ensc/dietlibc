#include <stdio.h>

#include "_dl_int.h"

int _dl_apply_relocate(struct _dl_handle* dh, Elf32_Rel *rel) {
  int ret=0;
  Elf32_Addr v = (Elf32_Addr) dh->mem_base;
  Elf32_Addr *loc;

  rel = (void*)rel+(long)v;
  loc = (Elf32_Addr *)(v+rel->r_offset);

  printf("_dl_apply_relocate %d @ %08lx\n",ELF32_R_TYPE(rel->r_info),(unsigned long)loc);

  switch (ELF32_R_TYPE(rel->r_info)) {
  case R_386_NONE:
    break;

  case R_386_32:
    *loc += v;
    break;

  case R_386_PLT32:
  case R_386_PC32:
//    *loc += v - dot;
    break;

  case R_386_GLOB_DAT:
  case R_386_JMP_SLOT:
    *loc = (unsigned long)_dl_sym(dh, ELF32_R_SYM(rel->r_info));
    break;

  case R_386_RELATIVE:
    *loc += (unsigned long)dh->mem_base;
    break;

#if 0
  case R_386_GOTPC:
    assert(got != 0);
    *loc += got - dot;
    break;

  case R_386_GOT32:
    assert(isym != NULL);
    if (!isym->gotent.reloc_done)
    {
      isym->gotent.reloc_done = 1;
      *(Elf32_Addr *)(ifile->got->contents + isym->gotent.offset) = v;
    }
    *loc += isym->gotent.offset;
    break;

  case R_386_GOTOFF:
    assert(got != 0);
    *loc += v - got;
    break;
#endif

  default:
    ret = 1;
    break;

  }
  return ret;
}

int _dl_relocate(struct _dl_handle* dh, Elf32_Rel *rel, int num) {
  int i;
  for (i=0;i<num;i++) {
    _dl_apply_relocate(dh,rel+i);
  }
  return 0;
}

