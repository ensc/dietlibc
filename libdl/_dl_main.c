#ifdef __OD_CLEAN_ROOM

#define __DIET_LD_SO__

/*
 * this is the dietlibc libdl & dynamic-linker
 *
 * NEED to be compiled with -fPIC ...
 */
#include <sys/mman.h>
#include <sys/stat.h>
#include "_dl_int.h"
#include "_dl_rel.h"

void _start(void);	/* entry of lib... */

static void (*fini_entry)(void)=0;
static char **_dl_environ=0;
static unsigned long loadaddr=0;
static unsigned long prog_entry=0;

static Elf_Phdr*prog_ph;
static unsigned long prog_ph_size;
static unsigned long prog_ph_num;

static unsigned long at_uid;
static unsigned long at_euid;
static unsigned long at_gid;
static unsigned long at_egid;
static unsigned long at_pagesize;

#warning "Ignore the warning like '`_dl_jump' used but never defined'"
/* declare static otherwise we have a GOT access BEFOR we have the resolver */
static void _dl_jump(void);

/* this are the "local syscalls" */
void _dl_sys_exit(int val);
int _dl_sys_read(int fd,char*buf,unsigned long len);
int _dl_sys_write(int fd,char*buf,unsigned long len);
int _dl_sys_open(const char*filename,int flags,int mode);
int _dl_sys_close(int fd);
void*_dl_sys_mmap(void*start,unsigned long length,int prot,int flags,int fd,unsigned long offset);
int _dl_sys_munmap(void*start,unsigned long length);
int _dl_sys_mprotect(const void*addr,unsigned long len,int prot);
int _dl_sys_fstat(int filedes, struct stat *buf);

extern char*strdup(const char*s);

#ifdef __i386__

asm(".text
.type _start,@function
_start:
	movl	%esp, %ebp		# save stack
	movl	(%ebp), %ecx		# argc
	leal	4(%ebp), %esi		# argv
	leal	4(%esi,%ecx,4), %eax	# envp

# PIC code
	call	getpic
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx

# for calculation of load addr, get 'relocated' address of _DYNAMIC
	leal	_DYNAMIC@GOTOFF(%ebx), %edx

# put parameter on stack and call _dl_main
	pushl	%edx
	pushl	%eax
	pushl	%esi
	pushl	%ecx
	call	_dl_main

# restore stack
	movl	%ebp, %esp

# get fini pointer
	movl	fini_entry@GOTOFF(%ebx), %edx

# clear callee-save-register like kernel
	xorl	%ebx, %ebx
	xorl	%ebp, %ebp
	xorl	%edi, %edi
	xorl	%esi, %esi

# jump to program entry point
	jmp	*%eax

_dl_sys_read:
	movb	$3,%al
	jmp	_dl_sys_call3
_dl_sys_write:
	movb	$4,%al
	jmp	_dl_sys_call3
_dl_sys_open:
	movb	$5,%al
	jmp	_dl_sys_call3
_dl_sys_close:
	movb	$6,%al
	jmp	_dl_sys_call3
_dl_sys_mmap:
	movb	$90,%al
	leal	4(%esp),%edx
	pushl	%edx
	call	_dl_sys_call3
	popl	%ecx
	ret
_dl_sys_munmap:
	movb	$91,%al
	jmp	_dl_sys_call3
_dl_sys_fstat:
	movb	$108,%al
	jmp	_dl_sys_call3
_dl_sys_mprotect:
	movb	$125,%al
	jmp	_dl_sys_call3
_dl_sys_exit:
	movl	$1,%eax
_dl_sys_call3:
	movzbl	%al,%eax
	pushl	%ebx
	movl	%esp,%ebx
	movl	16(%ebx),%edx
	movl	12(%ebx),%ecx
	movl	8(%ebx),%ebx
	int	$0x80
	popl	%ebx
	ret

#.global _dl_jump
.type	_dl_jump,@function
_dl_jump:
	pushl	%eax		# save register args...
	pushl	%ecx
	pushl	%edx

	push	16(%esp)	# 2. arg from plt
	push	16(%esp)	# 1. arg from plt
	call	do_resolve
	add	$8, %esp

	popl	%edx		# restore register args...
	popl	%ecx

	xchgl	%eax, (%esp)	# restore eax and save function pointer (for return)

	ret	$8		# remove arguments from plt and jump to REAL function

# GET Position In Code :)
getpic:	movl	(%esp), %ebx
	ret
");

static inline unsigned long* get_got(void) {
  register unsigned long *got asm ("%ebx");
  return got;
}

static inline int work_on_pltgot(struct _dl_handle*dh) {
  if ((dh->plt_rel)&&(!(dh->flags&RTLD_NOW))) {
    unsigned long*tmp=dh->pltgot;
    /* GOT */
    tmp[0]+=(unsigned long)dh->mem_base;	/* reloc dynamic pointer */
    tmp[1] =(unsigned long)dh;			/* the handle */
    tmp[2] =(unsigned long)(_dl_jump);		/* sysdep jump to do_resolve */
  }
  return 0;
}

#elif __arm__

asm(".text
.type _start,function
_start:
	mov	r4, sp
	mov	fp, #0			@ start new stack frame

	ldr	a1, [sp], #4		@ argc
	mov	a2, sp			@ argv

	add	a3, a2, a1, lsl #2	@ envp
	add	a3, a3, #4

	ldr	sl, .L_got		@ PIC code
1:	add	sl, pc, sl

	ldr	a4, .L_la		@ get 'relocated' address of _DYNAMIC
	add	a4, a4, sl

	bl	_dl_main		@ call _dl_main

	mov	sp, r4

	mov	lr, a1			@ save program entry point

	ldr	a1, [pc, #.L_fe-(.+8)]	@ agrument 1: global fini entry
	ldr	a1, [sl, a1]

	mov	pc, lr

.L_got:	.long	_GLOBAL_OFFSET_TABLE_-(1b+8)
.L_la:	.long	_DYNAMIC(GOTOFF)
.L_fe:	.long	fini_entry(GOTOFF)

_dl_sys_exit:
	swi	#0x900001		@ exit
	eor	lr, lr, lr		@ OR DIE !
	mov	pc, lr

_dl_sys_read:
	swi	#0x900003		@ read
	mov	pc, lr
_dl_sys_write:
	swi	#0x900004		@ write
	mov	pc, lr
_dl_sys_open:
	swi	#0x900005		@ open
	mov	pc, lr
_dl_sys_close:
	swi	#0x900006		@ close
	mov	pc, lr
_dl_sys_mmap:
	stmdb	sp!,{r0,r1,r2,r3}
	mov	r0, sp
	swi	#0x900090		@ mmap
	add	sp, #16
	mov	pc, lr
_dl_sys_munmap:
	swi	#0x900091		@ munmap
	mov	pc, lr
_dl_sys_fstat:
	swi	#0x900108		@ fstat
	mov	pc, lr
_dl_sys_mprotect:
	swi	#0x900125		@ mprotect
	mov	pc, lr

@.global _dl_jump
.type	_dl_jump,function
_dl_jump:
	stmdb	sp!, {r0, r1, r2, r3}	@ save arguments

	sub	r1, ip, lr		@ dyntab entry
	sub	r1, r1, #4
	add	r1, r1, r1

	ldr	r0, [lr, #-4]		@ dynlib handle

	bl	do_resolve

	mov	r12, r0
	ldmia	sp!, {r0, r1, r2, r3, lr} @ restore arguments
	mov	pc, r12
");

static inline unsigned long* get_got(void) {
  register unsigned long *got asm ("sl");
  return got;
}

static inline int work_on_pltgot(struct _dl_handle*dh) {
  if ((dh->plt_rel)&&(!(dh->flags&RTLD_NOW))) {
    unsigned long*tmp=dh->pltgot;
    /* GOT */
    tmp[0]+=(unsigned long)dh->mem_base;	/* reloc dynamic pointer */
    tmp[1] =(unsigned long)dh;			/* the handle */
    tmp[2] =(unsigned long)(_dl_jump);		/* sysdep jump to do_resolve */
  }
  return 0;
}

#else
#error "libdl: arch not supported"
#endif

static void*_dl_load(const char*fn,const char*pathname,int fd,int flags);

/* here do the code includes */

/* strncpy */
static char*strncpy(register char*s,register const char*t,register unsigned long n) {
  char *dest=s;
  for(;n;--n) {
    char ch=*t;
    *s=ch;
    if (ch==0) return dest;
    ++s; ++t;
  }
  return 0;
}

/* strlen.c */
static unsigned long strlen(register const char*s) {
  register unsigned long i;
  if (!s) return 0;
  for (i=0; *s; ++s) ++i;
  return i;
}

/* strcmp.c */
static int strcmp(register const unsigned char*s,register const unsigned char*t) {
  register char x;
  for (;;) {
    x = *s; if (x != *t) break; if (!x) break; ++s; ++t;
  }
  return ((int)(unsigned int)x) - ((int)(unsigned int)*t);
}

/* strcspn.c */
static unsigned long strcspn(const char*s,const char*reject) {
  unsigned long l=0;
  int a=1,i,al=strlen(reject);
  while((a)&&(*s)) {
    for(i=0;(a)&&(i<al);++i) if (*s==reject[i]) a=0;
    if (a) ++l;
    ++s;
  }
  return l;
}

/* memcpy.c */
static void*memcpy(void*dst,const void*src,unsigned long count) {
  register char *d=dst;
  register const char *s=src;
  ++count;
  while (--count) {
    *d = *s;
    ++d; ++s;
  }
  return dst;
}

/* memset.c */
static void*memset(void*dst,int ch,unsigned long count) {
  register char *d=dst;
  ++count;
  while (--count) {
    *d=ch;
    ++d;
  }
  return dst;
}

/* memcmp.c */
static int memcmp(register const unsigned char*s,register const unsigned char*t,unsigned long count) {
  register int r;
  ++count;
  while(--count) {
    if ((r=(*s-*t))) return r;
    ++s;
    ++t;
  }
  return 0;
}

/* getenv.c */
static char*getenv(const char*env) {
  unsigned int i,len=strlen(env);
  for (i=0;_dl_environ[i];++i) {
    if ((memcmp(_dl_environ[i],env,len)==0) && (_dl_environ[i][len]=='='))
      return _dl_environ[i]+len+1;
  }
  return 0;
}

/* basic debug output functions */
static void pf(const char*s) { _dl_sys_write(2,(void*)s,strlen(s)); }
static void ph(unsigned long l) {
  const int max=(sizeof(unsigned long)<<1);
  unsigned char buf[16];
  int i;
  for (i=max;i;l>>=4) {
    register unsigned long v='0'|(l&15);
    if (v>'9') v+=0x27;
    buf[--i]=v;
  }
  _dl_sys_write(2,buf,max);
}

/* the never free strdup (internal) */
static unsigned long _dl_lib_strdup_len=0;
static char*_dl_lib_strdup_str;
static char*_dl_lib_strdup(const char*s) {
  char*ret=_dl_lib_strdup_str;
  unsigned long l=strlen(s)+1;
  if (_dl_lib_strdup_len<l) {
    ret=(char*)_dl_sys_mmap(0,at_pagesize,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
    _dl_lib_strdup_len=at_pagesize;
  }
  _dl_lib_strdup_str=ret+l;
  _dl_lib_strdup_len-=l;
  memcpy(ret,s,l);
  return ret;
}

/* now reuse some unchanged sources */
#include "dlerror.c"
#include "_dl_alloc.c"

#include "dlsym.c"
#include "_dl_sym.c"

#include "_dl_search.c"

#include "_dl_open.c"
#include "dlopen.c"

#include "_dl_relocate.c"
#include "_dl_queue.c"

#include "dlclose.c"

/* back to the "new" implementation */
static void tt_fini(void) {
  struct _dl_handle*tmp;
#ifdef DEBUG
  pf("dyn fini\n");
#endif
  for(tmp=_dl_root_handle;tmp;tmp=tmp->next)
    if (tmp->fini) tmp->fini();
}

/* exit ! */
static void _DIE_() { _dl_sys_exit(213); }

/* lazy function resolver */
static unsigned long do_resolve(struct _dl_handle*dh,unsigned long off) {
  _dl_rel_t *tmp = ((void*)dh->plt_rel)+off;
  int sym=ELF_R_SYM(tmp->r_info);
  register unsigned long sym_val;

  if (0) sym_val=(unsigned long)do_resolve; /* TRICK: no warning */

  /* modify GOT for REAL symbol */
  sym_val=(unsigned long)_dl_sym(dh,sym);
  *((unsigned long*)(dh->mem_base+tmp->r_offset))=sym_val;

  /* JUMP (arg sysdep...) */
  if (sym_val) return sym_val;
  /* can't find symbol */
  return (unsigned long)_DIE_;
}

/* library loader */

/* ELF -> MMAP permissions */
static inline int map_flags(int flags) {
  int perm = 0;
  if (flags & PF_X) perm|=PROT_EXEC;
  if (flags & PF_R) perm|=PROT_READ;
  if (flags & PF_W) perm|=PROT_WRITE;
  return perm;
}

/* a simple mmap wrapper */
static inline void*do_map_in(void*base,unsigned long length,int flags,int fd,unsigned long offset) {
  register int op = MAP_PRIVATE;
  if (base) op|=MAP_FIXED;
  return _dl_sys_mmap(base, length, map_flags(flags), op, fd, offset);
}

/* map a library into memory */
#define _ELF_DWN_ROUND(ps,n)	((n)&(~((ps)-1)))
#define _ELF_UP_ROUND(ps,n)	((((n)&((ps)-1))?(ps):0)+_ELF_DWN_ROUND((ps),(n)))
#define _ELF_RST_ROUND(ps,n)	((n)&((ps)-1))
static struct _dl_handle*_dl_map_lib(const char*fn,const char*pathname,int fd,int flags) {
  struct _dl_handle*ret=0;
  int i;
  unsigned char buf[1024];
  char *m=0,*d=0;

  unsigned long l;
  struct stat st;

  Elf_Ehdr*eeh;
  Elf_Phdr*eph;

  int ld_nr=0;
  Elf_Phdr*ld[4]={0,0,0,0};
  Elf_Phdr*dyn=0;

  if (fd==-1) return 0;

  if (_dl_sys_fstat(fd,&st)<0) {
err_out_close:
    _dl_sys_close(fd);
    _dl_error_data=fn;
    _dl_error=2;
    return 0;
  } else {
    /* use st_dev and st_ino for identification */
  }

  if (_dl_sys_read(fd,buf,1024)<128) goto err_out_close;

  eeh=(Elf_Ehdr*)buf;
  eph=(Elf_Phdr*)&buf[eeh->e_phoff];

  for (i=0;i<eeh->e_phnum;++i) {
    if (eph[i].p_type==PT_LOAD) {
      if (ld_nr>3) goto err_out_close;
      ld[ld_nr++]=eph+i;
    }
    if (eph[i].p_type==PT_DYNAMIC) {
      dyn=eph+i;
    }
  }

  if (ld_nr==1) {
    unsigned long offset=_ELF_DWN_ROUND(at_pagesize,ld[0]->p_offset);
    unsigned long off   =_ELF_RST_ROUND(at_pagesize,ld[0]->p_offset);
    unsigned long length=_ELF_UP_ROUND(at_pagesize,ld[0]->p_memsz+off);
    ret=_dl_get_handle();
    m=(char*)do_map_in(0,length,ld[0]->p_flags,fd,offset);
    if (m==MAP_FAILED) goto err_out_free;
    /* zero pad bss */
    l=ld[0]->p_offset+ld[0]->p_filesz;
    memset(m+l,0,length-l);

    ret->mem_base=m;
    ret->mem_size=length;
  }
  else if (ld_nr==2) { /* aem... yes Quick & Really Dirty / for the avarage 99% */
//    unsigned long text_addr = _ELF_DWN_ROUND(at_pagesize,ld[0]->p_vaddr);	/* do we need this ? */
    unsigned long text_offset=_ELF_DWN_ROUND(at_pagesize,ld[0]->p_offset);
    unsigned long text_off   =_ELF_RST_ROUND(at_pagesize,ld[0]->p_offset);
    unsigned long text_size  =_ELF_UP_ROUND(at_pagesize,ld[0]->p_memsz+text_off);

    unsigned long data_addr  =_ELF_DWN_ROUND(at_pagesize,ld[1]->p_vaddr);
    unsigned long data_offset=_ELF_DWN_ROUND(at_pagesize,ld[1]->p_offset);
    unsigned long data_off   =_ELF_RST_ROUND(at_pagesize,ld[1]->p_offset);
    unsigned long data_size  =_ELF_UP_ROUND(at_pagesize,ld[1]->p_memsz+data_off);
    unsigned long data_fsize =_ELF_UP_ROUND(at_pagesize,ld[1]->p_filesz+data_off);

    ret=_dl_get_handle();
    /* mmap all mem_blocks for *.so */
    m=(char*)do_map_in(0,text_size+data_size,ld[0]->p_flags,fd,text_offset);
    if (m==MAP_FAILED) {
err_out_free:
      _dl_free_handle(ret);
      _dl_sys_close(fd);
      return 0;
    }

    /* release data,bss part */
    _dl_sys_mprotect(m+data_addr,data_size,PROT_NONE);

    /* mmap data,bss part */
    d=(char*)do_map_in(m+data_addr,data_fsize,ld[1]->p_flags,fd,data_offset);

    /* zero pad bss */
    l=data_off+ld[1]->p_filesz;
    memset(d+l,0,data_fsize-l);
    /* more bss ? */
    if (data_size>data_fsize) {
      l=data_size-data_fsize;
      _dl_sys_mmap(d+data_fsize,l,PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS,-1,0);
    }

    ret->mem_base=m;
    ret->mem_size=text_size+data_size;
  }
  else {
    _dl_error_data=fn;
    _dl_error=7;
  }

  if (ret) {
    ++ret->lnk_count;
    if (flags&RTLD_USER)
      ret->name=strdup(fn);
    else
      ret->name=_dl_lib_strdup(fn);
    ret->flags=flags;
    ret->dynamic=(Elf_Dyn*)(m+dyn->p_vaddr);
  }

  _dl_sys_close(fd);
  return ret;
}

/* dynamic section parser */
static struct _dl_handle* _dl_dyn_scan(struct _dl_handle*dh,Elf_Dyn*_dynamic) {
  void(*init)(void)=0;

  _dl_rel_t* plt_rel=0;
  unsigned long  plt_relsz=0;

  _dl_rel_t* rel=0;
  unsigned long relent=0;
  unsigned long relsize=0;

  int i;

#ifdef DEBUG
  pf(__func__": pre dynamic scan "); ph((unsigned long)dh); pf("\n");
#endif
  for(i=0;_dynamic[i].d_tag;++i) {
    switch(_dynamic[i].d_tag) {
      /* this depends on dyn_str_tab -> second run */
    case DT_NEEDED:
    case DT_SONAME:
      break;

      /* BASIC DYNAMIC STUFF */
    case DT_HASH:
      dh->hash_tab = (unsigned long*)(dh->mem_base+_dynamic[i].d_un.d_ptr);
#ifdef DEBUG
      pf(__func__": have hash @ "); ph((long)dh->hash_tab); pf("\n");
#endif
      break;
    case DT_SYMTAB:
      dh->dyn_sym_tab = (Elf_Sym*)(dh->mem_base+_dynamic[i].d_un.d_ptr);
#ifdef DEBUG
      pf(__func__": have dyn_sym_tab @ "); ph((long)dh->dyn_sym_tab); pf("\n");
#endif
      break;
    case DT_STRTAB:
      dh->dyn_str_tab = (char*)(dh->mem_base+_dynamic[i].d_un.d_ptr);
#ifdef DEBUG
      pf(__func__": have dyn_str_tab @ "); ph((long)dh->dyn_str_tab); pf("\n");
#endif
      break;

      /* DYNAMIC INIT/FINI (constructors/destructors) */
    case DT_FINI:
      dh->fini = (void(*)(void))(dh->mem_base+_dynamic[i].d_un.d_val);
#ifdef DEBUG
      pf(__func__": have fini @ "); ph((long)dh->fini); pf("\n");
#endif
      break;
    case DT_INIT:
      init = (void(*)(void))(dh->mem_base+_dynamic[i].d_un.d_val);
#ifdef DEBUG
      pf(__func__": have init @ "); ph((long)init); pf("\n");
#endif
      break;

      /* PLT RELOCATION */
    case DT_PLTGOT:
      dh->pltgot = (unsigned long*)(dh->mem_base+_dynamic[i].d_un.d_val);
#ifdef DEBUG
      pf(__func__": have plt/got @ "); ph((long)dh->pltgot); pf("\n");
#endif
      break;
    case DT_PLTREL:
      if (_dynamic[i].d_un.d_val!=_DL_REL_T) {
#ifdef DEBUG
	pf(__func__": have incompatible relocation type\n");
#endif
	_dl_error_data=dh->name;
	_dl_error=6;
	return 0;
      }
      break;
    case DT_JMPREL:
      plt_rel = (_dl_rel_t*)(dh->mem_base+_dynamic[i].d_un.d_val);
      dh->plt_rel = plt_rel;
#ifdef DEBUG
      pf(__func__": have jmprel @ "); ph((long)plt_rel); pf("\n");
#endif
      break;
    case DT_PLTRELSZ:
      plt_relsz = _dynamic[i].d_un.d_val;
#ifdef DEBUG
      pf(__func__": have pltrelsize @ "); ph((long)plt_relsz); pf("\n");
#endif
      break;

      /* BASIC RELOCATION */
    case DT_REL:
      rel = (_dl_rel_t*)(dh->mem_base+_dynamic[i].d_un.d_val);
#ifdef DEBUG
      pf(__func__": have rel @ "); ph((long)rel); pf("\n");
#endif
      break;
    case DT_RELENT:
      relent=_dynamic[i].d_un.d_val;
#ifdef DEBUG
      pf(__func__": have relent  @ "); ph((long)relent); pf("\n");
#endif
      break;
    case DT_RELSZ:
      relsize=_dynamic[i].d_un.d_val;
#ifdef DEBUG
      pf(__func__": have relsize @ "); ph((long)relsize); pf("\n");
#endif
      break;


      /* TEXT RELOCATIONS POSSIBLE -> NO SHARED OBJECT */
    case DT_TEXTREL:
#ifdef DEBUG
      pf(__func__": found possible textrelocation -> "); pf(dh->name); pf("is no compiled as a shared library\n");
#endif
      _dl_error_data=dh->name;
      _dl_error=3;
      return 0;
      break;

      /* OTHERS */
    default:
#ifdef DEBUG
#if 0
      pf(__func__": unknown "); ph(_dynamic[i].d_tag); pf(", "); ph(_dynamic[i].d_un.d_val); pf("\n");
#endif
#endif
    }
  }

  for(i=0;_dynamic[i].d_tag;i++) {
    if (dh->name) {	/* librabry can have a SONAME */
      if (_dynamic[i].d_tag==DT_SONAME) {
#ifdef DEBUG
	pf(__func__": pre  soname: "); pf(dh->name); pf("\n");
#endif
	if (dh->flags&RTLD_USER) free(dh->name);
	dh->flags&=~RTLD_NOSONAME;
	dh->name = dh->dyn_str_tab+_dynamic[i].d_un.d_val;
#ifdef DEBUG
	pf(__func__": have soname: "); pf(dh->name); pf("\n");
#endif
      }
    }
    else {		/* programs can have a LD_RUN_PATH */
      if (_dynamic[i].d_tag==DT_RPATH) {
	register char *rpath=dh->dyn_str_tab+_dynamic[i].d_un.d_val;
	_dl_search_rpath=rpath;
#ifdef DEBUG
	pf(__func__": have runpath: "); pf(rpath); pf("\n");
#endif
      }
    }
  }

#ifdef DEBUG
  pf(__func__": post dynamic scan "); ph((unsigned long)dh); pf("\n");
#endif

  if (work_on_pltgot(dh)) {
    _dl_error_data=dh->name;
    _dl_error=3;
    return 0;
  }
#if 0
  if ((tmp=_dlsym(dh,"_GLOBAL_OFFSET_TABLE_"))) {
    dh->got=tmp;
#ifdef DEBUG
    pf(__func__": found a GOT @ "); ph((unsigned long)tmp); pf("\n");
#endif
    /* GOT */
    tmp[0]+=(unsigned long)dh->mem_base;	/* reloc dynamic pointer */
    tmp[1] =(unsigned long)dh;
    tmp[2] =(unsigned long)(_dl_jump);		/* sysdep jump to do_resolve */
  }
  else {
    _dl_error_data=dh->name;
    _dl_error=3;
    return 0;
  }
#endif

#ifdef DEBUG
  pf(__func__": pre load depending libraries "); ph((unsigned long)dh); pf("\n");
#endif
  /* load depending libs */
  for(i=0;_dynamic[i].d_tag;++i) {
    if (_dynamic[i].d_tag==DT_NEEDED) {
      char *lib_name=dh->dyn_str_tab+_dynamic[i].d_un.d_val;
#ifdef DEBUG
      pf(__func__": needed for this lib: "); pf(lib_name); pf("\n");
#endif
      _dl_queue_lib(lib_name,dh->flags);
    }
  }
#ifdef DEBUG
  pf(__func__": pre open depending libraries 2 "); ph((unsigned long)dh); pf("\n");
#endif
  if (_dl_open_dep()) {
    return 0;
  }

#ifdef DEBUG
  pf(__func__": post load depending libraries, pre resolve "); ph((unsigned long)dh); pf("\n");
#endif

  /* relocation */
  if (rel) {
#ifdef DEBUG
    pf(__func__": try to relocate some values\n");
#endif
    if (_dl_relocate(dh,rel,relsize/relent)) return 0;
  }

  /* do PTL / GOT relocation */
  if (plt_rel) {
    _dl_rel_t *tmp,*max=((void*)plt_rel)+plt_relsz;
#ifdef DEBUG
    pf(__func__": rel plt/got\n");
#endif
    for(tmp=plt_rel;tmp<max;(char*)tmp=((char*)tmp)+sizeof(_dl_rel_t)) {
      if ((dh->flags&RTLD_NOW)) {
	unsigned long sym=(unsigned long)_dl_sym(dh,ELF_R_SYM(tmp->r_info));
	if (sym) *((unsigned long*)(dh->mem_base+tmp->r_offset))=sym;
	else {
	  _dl_error_data=dh->name;
	  _dl_error=4;
	  return 0;
	}
      }
      else
	_DL_REL_PLT(dh->mem_base,tmp);
#ifdef DEBUG
      pf(__func__": rel @ "); ph((long)dh->mem_base+tmp->r_offset); pf(" with type ");
      ph(ELF_R_TYPE(tmp->r_info)); pf(" and sym "); ph(ELF_R_SYM(tmp->r_info));
      pf(" -> "); ph(*((unsigned long*)(dh->mem_base+tmp->r_offset))); pf("\n");
#endif
    }
  }

#ifdef DEBUG
  pf(__func__": post resolve, pre init "); ph((unsigned long)dh); pf("\n");
#endif
  if (init) init();
#ifdef DEBUG
  pf(__func__": post init "); ph((unsigned long)dh); pf("\n");
#endif

  return dh;
}

static void*_dl_load(const char*fn,const char*pathname,int fd,int flags) {
  struct _dl_handle*ret=0;
  if ((ret=_dl_map_lib(fn,pathname,fd,flags))) {
    ret=_dl_dyn_scan(ret,ret->dynamic);
  }
  return ret;
}


/* ELF AUX parser */
static void _dl_elfaux(register unsigned long*ui) {
  register struct elf_aux {
    unsigned long type;
    unsigned long val;
  } *ea;

  while (*ui) ++ui;
  /* now *ui points to the tailing NULL-pointer of the envirioment */

  /* walk the elf_aux table */
  for (ea=(struct elf_aux*)(ui+1); ea->type; ++ea) {
    switch (ea->type) {
    case AT_EXECFD:	/* 2 */
      /* DIE! DIE! DIE! */
      pf("kernel gives us an unsupported binary load type...\n");
      _dl_sys_exit(42);
      break;

    case AT_PHDR:	/* 3 */
      prog_ph=(Elf_Phdr*)ea->val;
#ifdef DEBUG
      pf("program header @ "); ph(ea->val); pf("\n");
#endif
      break;
    case AT_PHENT:	/* 4 */
      prog_ph_size=ea->val;
#ifdef DEBUG
      pf("program header size "); ph(ea->val); pf("\n");
#endif
      break;
    case AT_PHNUM:	/* 5 */
      prog_ph_num=ea->val;
#ifdef DEBUG
      pf("program header # "); ph(ea->val); pf("\n");
#endif
      break;

    case AT_PAGESZ:	/* 6 */
      at_pagesize=ea->val;
#ifdef DEBUG
      pf("page size "); ph(ea->val); pf("\n");
#endif
      break;

    case AT_BASE:	/* 7 */
      loadaddr=ea->val;
#ifdef DEBUG
      pf("interpreter base: "); ph(ea->val); pf("\n");
#endif
      break;

#if 0
    case AT_FLAGS:	/* 8 */
#ifdef DEBUG
      pf("flags "); ph(ea->val); pf("\n");
#endif
      break;
#endif

    case AT_ENTRY:	/* 9 */
      prog_entry=ea->val;
#ifdef DEBUG
      pf("start program  @ "); ph(ea->val); pf("\n");
#endif
      break;

    case AT_NOTELF:	/* 10 */
      pf("this is an ELF-loader... and therefor can't handle anything else.\n");
      _dl_sys_exit(42);
      break;

    case AT_UID:	/* 11 */
      at_uid=ea->val;
#ifdef DEBUG
      pf(" UID: "); ph(ea->val); pf("\n");
#endif
      break;
    case AT_EUID:	/* 12 */
      at_euid=ea->val;
#ifdef DEBUG
      pf("EUID: "); ph(ea->val); pf("\n");
#endif
      break;
    case AT_GID:	/* 13 */
      at_gid=ea->val;
#ifdef DEBUG
      pf(" GID: "); ph(ea->val); pf("\n");
#endif
      break;
    case AT_EGID:	/* 14 */
      at_egid=ea->val;
#ifdef DEBUG
      pf("EGID: "); ph(ea->val); pf("\n");
#endif
      break;

#if 0
    case AT_PLATFORM:	/* 15 */
#ifdef DEBUG
      pf("CPU: "); ph(ea->val); pf("\n");
#endif
      break;
    case AT_HWCAP:	/* 16 */
#ifdef DEBUG
      pf("CPU capabilities: "); ph(ea->val); pf("\n");
#endif
      break;
    case AT_CLKTCK:	/* 17 */
#ifdef DEBUG
      pf("CLK per sec "); ph( ea->val); pf("\n");
#endif
      break;
    case AT_FPUCW:	/* 18 */
#ifdef DEBUG
      pf("FPU control word "); ph( ea->val); pf("\n");
#endif
      break;
#endif

    default:
      break;
    }
  }
}


/* start of libdl dynamic linker */
static unsigned long _dl_main(int argc,char*argv[],char*envp[],unsigned long _dynamic) {
  unsigned long*got;
  struct _dl_handle*prog,*mydh;
  struct _dl_handle my_dh;
  Elf_Dyn*prog_dynamic=0;
  int i;

  if (0) _dl_main(argc,argv,envp,_dynamic); /* TRICK: no warning */

  /* prepare to bootstarp the relocations */
  got=get_got();
  _dl_environ=envp;

  /* run elf_aux (kernel provided misc data) */
  _dl_elfaux((unsigned long*)envp);

  if (loadaddr==0) {
    pf("\ndiet libdl.so/dynamic-linker can't be started as a program !\n\n SORRY...\n\n");
    return (unsigned long)_DIE_;
  }

  memset(&my_dh,0,sizeof(my_dh));
  my_dh.mem_base=(char*)loadaddr;
  my_dh.mem_size=0;
  my_dh.lnk_count=1024;
  my_dh.name="libdl.so";
  my_dh.flags=LDSO_FLAGS;

  got[1]=0;			/* NOT YET (my_dh) */
  got[2]=(unsigned long)_DIE_;	/* NO lazy symbol resolver as long as we are not ready */

#ifdef DEBUG
  pf(__func__": pre scan\n");
#endif
  /* bootstrap relocation */
  if (_dl_dyn_scan(&my_dh,(Elf_Dyn*)_dynamic)==0) {
    pf("error with dyn_scan myself\n");
    return (unsigned long)_DIE_;
  }
#ifdef DEBUG
  pf(__func__": post scan\n");
#endif

  /* now we are save to use anything :) (hopefully) */

  fini_entry=tt_fini;

  prog=_dl_get_handle();

#ifdef DEBUG
  pf(__func__": ugly, ugly, COPY pregenerated handle to real handle\n");
#endif
  mydh=_dl_get_handle();
  {
    register struct _dl_handle*tmp=mydh->prev;
    memcpy(mydh,&my_dh,sizeof(struct _dl_handle));
    mydh->prev=tmp;
  }
  got[1]=(unsigned long)mydh;

#ifdef DEBUG
  pf(__func__": MORE ugly: prepare program...\n");
#endif
  for(i=0;(i<prog_ph_num);++i) {
    if (prog_ph[i].p_type==PT_DYNAMIC) {
      prog_dynamic=(Elf_Dyn*)prog_ph[i].p_vaddr;
      break;
    }
  }
  if (prog_dynamic==0) {
    ph(0xe0000001);
    pf(" error with program: no dynamic section ?\n");
    return (unsigned long)_DIE_;
  }
  prog->name=0;
  prog->lnk_count=1024;
  prog->dynamic=prog_dynamic;
  prog->flags=LDSO_FLAGS;

#ifdef DEBUG
  pf(__func__": dyn_scan program...\n");
#endif
  if (_dl_dyn_scan(prog,(Elf_Dyn*)prog_dynamic)==0) {
    _dl_error_location="error in dyn_scan the program";
    pf(dlerror()); pf("\n");
    return (unsigned long)_DIE_;
  }

  /* now start the program */
#ifdef DEBUG
  pf(__func__": now jump to program entrypoint\n");
#endif
  return prog_entry;
}

#endif
