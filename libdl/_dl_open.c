#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <dlfcn.h>
#include <linux/elf.h>

#include "_dl_int.h"

#define _ELF_DWN_ROUND(ps,n)	((n)&(~((ps)-1)))
#define _ELF_UP_ROUND(ps,n)	((((n)&((ps)-1))?(ps):0)+ _ELF_DWN_ROUND((ps),(n)))
#define _ELF_RST_ROUND(ps,n)	((n)&((ps)-1))

/* this is an arch specific "return jump" for the relocation */
void _dl_jump();

/*
 * this file is a Q. & D. hack ... don't think this is bug free or meaningfull
 */

static void *do_map_in(void *base, unsigned long length, int flags, int fd, unsigned long offset)
{
  int perm = 0;
  if (flags & PF_X) perm|=PROT_EXEC;
  if (flags & PF_R) perm|=PROT_READ;
  if (flags & PF_W) perm|=PROT_WRITE;
  return mmap(base, length, perm, MAP_PRIVATE|((base)?MAP_FIXED:0), fd, offset);
}

unsigned long do_rel(struct _dl_handle * tmp_dl, unsigned long off)
{
  Elf32_Rel *tmp = ((void*)tmp_dl->plt_rel)+off;

  int sym=ELF32_R_SYM(tmp->r_info);

  register unsigned long sym_val;

#if 1
  printf("do_rel %08lx %08lx\n",(long)tmp_dl,off);
  printf("do_rel %08lx+%lx\n",(long)tmp_dl->plt_rel,off);
  printf("do_rel @ %08lx with type %d -> %d\n",(long)tmp->r_offset,ELF32_R_TYPE(tmp->r_info),sym);
  printf("do_rel sym %08lx\n",(long)tmp_dl->dyn_sym_tab[sym].st_value);
#endif

  /* modify GOT for REAL symbol */
  //sym_val=((unsigned long)(tmp_dl->mem_base+tmp_dl->dyn_sym_tab[sym].st_value));
  sym_val=(unsigned long)_dl_sym(tmp_dl,sym);
  *((unsigned long*)(tmp_dl->mem_base+tmp->r_offset))=sym_val;

  printf("do_rel sym %08lx\n",(long)sym_val);
  /* JUMP (arg sysdep...) */
  return sym_val;
}

void *_dl_open(const char*pathname, int fd, int flag)
{
  struct _dl_handle* dl_test=0;
  int ps=getpagesize();
  int i;
  unsigned char buf[1024];
  char *m=0,*d;
  struct _dl_handle *ret=0;

  unsigned long l;

  Elf32_Ehdr *eh;
  Elf32_Phdr *ph;

  int ld_nr=0;
  Elf32_Phdr *ld[4]={0,0,0,0};
  Elf32_Phdr *dyn=0;

  if (fd==-1) return 0;

  printf("_dl_open: %s\n",pathname);

  read(fd, buf, 1024);
  eh=(Elf32_Ehdr*)buf;
  ph=(Elf32_Phdr*)&buf[eh->e_phoff];

  for (i=0; i<eh->e_phnum; i++) {
    if (ph[i].p_type==PT_LOAD) {
      ld[ld_nr++]=ph+i;
    }
    if (ph[i].p_type==PT_DYNAMIC) {
      dyn=ph+i;
    }
  }

  /* get a little page for *.so administration
   * (in 99% the page pre text/rodata)
   */
//  dl_test = (struct _dl_handle *)mmap(0, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  dl_test = _dl_get_handle();

  if (ld_nr==1) {
    unsigned long offset = _ELF_DWN_ROUND(ps,ld[0]->p_offset);
    unsigned long off = _ELF_RST_ROUND(ps,ld[0]->p_offset);
    unsigned long length = _ELF_UP_ROUND(ps,ld[0]->p_memsz+off);
    m = (char*)do_map_in(0, length, ld[0]->p_flags, fd, offset);

    /* zero pad bss */
    l = ld[0]->p_offset+ld[0]->p_filesz;
    memset(m+l,0,length-l);

    dl_test->mem_base=m;
    dl_test->mem_size=length;
    dl_test->lnk_count=0;

    ret = dl_test;
  }
  else if (ld_nr==2) { /* aem... yes Quick & Really Dirty / for the avarage 99% */
    //unsigned long text_addr = _ELF_DWN_ROUND(ps,ld[0]->p_vaddr);
    unsigned long text_offset = _ELF_DWN_ROUND(ps,ld[0]->p_offset);
    unsigned long text_off = _ELF_RST_ROUND(ps,ld[0]->p_offset);
    unsigned long text_size = _ELF_UP_ROUND(ps,ld[0]->p_memsz+text_off);

    unsigned long data_addr = _ELF_DWN_ROUND(ps,ld[1]->p_vaddr);
    unsigned long data_offset = _ELF_DWN_ROUND(ps,ld[1]->p_offset);
    unsigned long data_off = _ELF_RST_ROUND(ps,ld[1]->p_offset);
    unsigned long data_size = _ELF_UP_ROUND(ps,ld[1]->p_memsz+data_off);
    unsigned long data_fsize = _ELF_UP_ROUND(ps,ld[1]->p_filesz+data_off);

    /* mmap all mem_blocks for *.so */
    l = text_size+data_size;

    dl_test->mem_size=l;

    m = (char*) do_map_in(0,l,ld[0]->p_flags,fd,text_offset);

    /* release data,bss part */
    mprotect(m+data_addr, data_size, PROT_NONE);

    /* mmap data,bss part */
    d = (char*) do_map_in(m+data_addr,data_fsize,ld[1]->p_flags,fd,data_offset);

    /* zero pad bss */
    l = data_off+ld[1]->p_filesz;
    memset(d+l,0,data_size-l);

    /* more bss ? */
    if (data_size>data_fsize) {
      l = data_size-data_fsize;
      mmap(d+data_fsize, l, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0);
    }

    dl_test->mem_base=m;
    dl_test->lnk_count=0;
    ret = dl_test;
  }

  printf("_dl_open pre resolv\n");
  if (ret) {
    Elf32_Dyn* dyn_tab = (void*)m+dyn->p_vaddr;
    void (*init)()=0;
    unsigned long* got=0;
    void* jmprel=0;
    int pltreltype=0;
    int pltrelsize=0;

    printf("_dl_open IN resolv\n");
    for(i=0;dyn_tab[i].d_tag;i++) {
      if (dyn_tab[i].d_tag==DT_HASH) {
	ret->hash_tab = (unsigned long*)(m+dyn_tab[i].d_un.d_ptr);
      }
      if (dyn_tab[i].d_tag==DT_SYMTAB) {
	ret->dyn_sym_tab = (Elf32_Sym*)(m+dyn_tab[i].d_un.d_ptr);
      }
      if (dyn_tab[i].d_tag==DT_STRTAB) {
	ret->dyn_str_tab = (char*)(m+dyn_tab[i].d_un.d_ptr);
      }
      if (dyn_tab[i].d_tag==DT_FINI) {
	ret->fini = (void(*)(void))(m+dyn_tab[i].d_un.d_val);
      }
      if (dyn_tab[i].d_tag==DT_INIT) {
	init = (void(*)(void))(m+dyn_tab[i].d_un.d_val);
	printf("init @ %08lx\n",(long)init);
      }
      if (dyn_tab[i].d_tag==DT_PLTGOT) {
	got=(unsigned long*)(m+dyn_tab[i].d_un.d_val);
	ret->got=got;
	printf("_dl_open have got @ %08lx\n",(long)got);
      }
      if (dyn_tab[i].d_tag==DT_PLTREL) {
	pltreltype=dyn_tab[i].d_un.d_val;
      }
      if (dyn_tab[i].d_tag==DT_PLTRELSZ) {
	pltrelsize=dyn_tab[i].d_un.d_val;
      }
      if (dyn_tab[i].d_tag==DT_JMPREL) {
	jmprel=(m+dyn_tab[i].d_un.d_val);
	ret->plt_rel=jmprel;
      }
    }
    printf("_dl_open post dynamic scan\n");
    /* GOT */
    got[0]+=(unsigned long)m;	/* reloc dynamic pointer */
    got[1]=(unsigned long)dl_test;
    got[2]=(unsigned long)(_dl_jump);	/* sysdep jump to _dl_rel */
    /* */

    if (pltreltype == DT_REL) {
      Elf32_Rel *tmp = jmprel;
      printf("_dlopen rel\n");
      for (;(char*)tmp<(((char*)jmprel)+pltrelsize);(char*)tmp=((char*)tmp)+sizeof(Elf32_Rel)) {
	*((unsigned long*)(m+tmp->r_offset))+=(unsigned long)m;
	printf("rel @ %08x with type %d -> %d\n",tmp->r_offset,ELF32_R_TYPE(tmp->r_info),ELF32_R_SYM(tmp->r_info));
      }
    }
    if (pltreltype == DT_RELA) {
      Elf32_Rela *tmp = jmprel;
      printf("_dlopen rela\n");
      for (;(char*)tmp<(((char*)jmprel)+pltrelsize);(char*)tmp=((char*)tmp)+sizeof(Elf32_Rel)) {
	*((unsigned long*)(m+tmp->r_offset))=(unsigned long)(m+tmp->r_addend);
	printf("rela @ %08x with type %d -> %d\n",tmp->r_offset,ELF32_R_TYPE(tmp->r_info),ELF32_R_SYM(tmp->r_info));
      }
    }

    // _dl_open depending libs ...
    printf("_dl_open post resolv, pre init\n");
    if (init) init();
  }
  printf("_dl_open post resolv, post init\n");

  close(fd);
  return ret;
}
