#ifdef __OD_CLEAN_ROOM

#define __DIET_LD_SO__

/*
 * this is the dietlibc libdl & dynamic-linker
 *
 * NEED to be compiled with -fPIC ...
 */
#include "_dl_int.h"
#include "_dl_rel.h"

void _start(void);	/* entry of lib... */

static void (*fini_entry)(void)=0;
static char **_dl_environ=0;
static unsigned long loadaddr=0;
static unsigned long prog_entry=0;

static unsigned long at_uid;
static unsigned long at_euid;
static unsigned long at_gid;
static unsigned long at_egid;

#warning "Ignore all warnings about 'used but never defined' functions..."
static void _dl_jump(void);
static void _dl_sys_exit(int val);
static int _dl_sys_write(int fd,char*buf,unsigned long len);
/* mmap, munmap, mprotect, fstat, open, read, close, */

#define pf(s) _dl_sys_write(2,(s),sizeof((s)))

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

# clear register like kernel
	xorl	%ecx, %ecx
	xorl	%ebx, %ebx
	xorl	%ebp, %ebp
	xorl	%edi, %edi
	xorl	%esi, %esi

# jump to program entry point
	jmp	*%eax

# test / debug code :)
_dl_sys_exit:
	movl	$1,%eax
	popl	%ebx
	int	$0x80
	hlt

_dl_sys_write:
	pushl	%ebx
	movl	%esp,%ebx
	movl	16(%ebx),%edx
	movl	12(%ebx),%ecx
	movl	8(%ebx),%ebx
	movl	$4,%eax
	int	$0x80
	popl	%ebx
	ret

#.global _dl_jump
.type	_dl_jump,@function
_dl_jump:
	pushl	%eax		# save register args...
	pushl	%ecx
	pushl	%edx

	push	20(%esp)	# 2. arg from plt
	push	20(%esp)	# 1. arg from plt
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
	swi	#1			@ exit
	eor	lr, lr, lr		@ OR DIE !
	mov	pc, lr

_dl_sys_write:
	swi	#4			@ write
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

#else
#error "libdl: arch not supported"
#endif


/* here do the code includes */


/* strncpy */
static char *strncpy(register char *s, register const char *t, register unsigned long n) {
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
static unsigned long strlen(register const char *s) {
  register unsigned long i;
  if (!s) return 0;
  for (i=0; *s; ++s) ++i;
  return i;
}

/* strncat.c */
static char *strncat(register char *s, register const char *t, unsigned long n) {
  char *dest=s;
  register char *max;
  s+=strlen(s);

  if ((max=s+n-1)<=s) goto fini;

  for (;;) {
    if (!(*s = *t)) break; if (++s==max) break; ++t;
  }
  *s=0;
fini:
  return dest;
}

/* strcmp.c */
static int strcmp(register const unsigned char *s,register const unsigned char *t) {
  register char x;
  for (;;) {
    x = *s; if (x != *t) break; if (!x) break; ++s; ++t;
  }
  return ((int)(unsigned int)x) - ((int)(unsigned int)*t);
}

/* memcpy.c */
static void*memcpy(void* dst, const void* src, unsigned long count) {
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
static void*memset(void *dst, int ch, unsigned long count) {
  register char *d=dst;
  ++count;
  while (--count) {
    *d=ch;
    ++d;
  }
  return dst;
}

/* memcmp.c */
static int memcmp(register const unsigned char *s,register const unsigned char *t,unsigned long count) {
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

#include "dlerror.c"
//#include "_dl_alloc.c"	/* ALLOC */
static struct _dl_handle*_dl_root_handle=0;

#include "dlsym.c"
#include "_dl_sym.c"

#include "_dl_relocate.c"
//#include "_dl_queue.c"	/* QUEUE */
#include "_dl_search.c"

//#include "dlopen.c"
//#include "dlclose.c"

static void tt_fini(void) {
  struct _dl_handle*tmp;
  DEBUG("dyn fini\n");
  for(tmp=_dl_root_handle;tmp;tmp=tmp->next)
    if (tmp->fini) tmp->fini();
}

/* exit ! */
static void _DIE_() { _dl_sys_exit(213); }
/* lazy function resolver */
static unsigned long do_resolve(struct _dl_handle *dh, unsigned long off) {
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

/* dynamic section parser */
static struct _dl_handle* _dl_dyn_scan(struct _dl_handle*dh,Elf_Dyn*_dynamic) {
  void(*init)(void)=0;
  unsigned long* tmp;

  _dl_rel_t* plt_rel=0;
  unsigned long  plt_relsz=0;

  _dl_rel_t* rel=0;
  unsigned long relent=0;
  unsigned long relsize=0;

  int i;

  DEBUG("_dl_dyn_scan pre dynamic scan %08lx\n",(unsigned long)dh);

  for(i=0;_dynamic[i].d_tag;i++) {
    switch(_dynamic[i].d_tag) {
      /* BASIC DYNAMIC STUFF */
    case DT_HASH:
      dh->hash_tab = (unsigned long*)(dh->mem_base+_dynamic[i].d_un.d_ptr);
      DEBUG("_dl_dyn_scan have hash @ %08lx\n",(long)dh->hash_tab);
      break;
    case DT_SYMTAB:
      dh->dyn_sym_tab = (Elf_Sym*)(dh->mem_base+_dynamic[i].d_un.d_ptr);
      DEBUG("_dl_dyn_scan have dyn_sym_tab @ %08lx\n",(long)dh->dyn_sym_tab);
      break;
    case DT_STRTAB:
      dh->dyn_str_tab = (char*)(dh->mem_base+_dynamic[i].d_un.d_ptr);
      DEBUG("_dl_dyn_scan have dyn_str_tab @ %08lx\n",(long)dh->dyn_str_tab);
      break;

      /* DYNAMIC INIT/FINI (constructors/destructors) */
    case DT_FINI:
      dh->fini = (void(*)(void))(dh->mem_base+_dynamic[i].d_un.d_val);
      DEBUG("_dl_dyn_scan have fini @ %08lx\n",(long)dh->fini);
      break;
    case DT_INIT:
      init = (void(*)(void))(dh->mem_base+_dynamic[i].d_un.d_val);
      DEBUG("_dl_dyn_scan have init @ %08lx\n",(long)dh->init);
      break;

      /* PLT RELOCATION */
    case DT_PLTGOT:
      dh->pltgot = (unsigned long*)(dh->mem_base+_dynamic[i].d_un.d_val);
      DEBUG("_dl_dyn_scan have plt got @ %08lx\n",(long)dh->pltgot);
      break;
    case DT_PLTREL:
      if (_dynamic[i].d_un.d_val!=_DL_REL_T) {
	DEBUG("_dl_dyn_scan have incompatible relocation type\n");
	_dl_error = 5;
	return 0;
      }
      break;
    case DT_JMPREL:
      plt_rel = (_dl_rel_t*)(dh->mem_base+_dynamic[i].d_un.d_val);
      dh->plt_rel = plt_rel;
      DEBUG("_dl_dyn_scan have jmprel @ %08lx\n",(long)plt_rel);
      break;
    case DT_PLTRELSZ:
      plt_relsz = _dynamic[i].d_un.d_val;
      DEBUG("_dl_load have pltrelsize @ %08lx\n",(long)plt_relsz);
      break;

      /* BASIC RELOCATION */
    case DT_REL:
      rel = (_dl_rel_t*)(dh->mem_base+_dynamic[i].d_un.d_val);
      DEBUG("_dl_dyn_scan have rel @ %08lx\n",(long)rel);
      break;
    case DT_RELENT:
      relent=_dynamic[i].d_un.d_val;
      DEBUG("_dl_dyn_scan have relent  @ %08lx\n",(long)relent);
      break;
    case DT_RELSZ:
      relsize=_dynamic[i].d_un.d_val;
      DEBUG("_dl_dyn_scan have relsize @ %08lx\n",(long)relsize);
      break;

      /* SHARED NAME */
    case DT_SONAME:
      if (dh->name) {
	dh->name = dh->dyn_str_tab+_dynamic[i].d_un.d_val;
	DEBUG("_dl_dyn_scan have soname: %s\n",dh->name);
      }
      break;


      /* TEXT RELOCATIONS POSSIBLE -> NO SHARED OBJECT */
    case DT_TEXTREL:
      DEBUG("_dl_dyn_scan found possible textrelocation -> %s is no compiled as a shared library\n",dh->name);
      _dl_error = 2;
      return 0;
      break;

      /* OTHERS */
    default:
      DEBUG("_dl_dyn_scan: unknown %d, %08lx\n",_dynamic[i].d_tag,_dynamic[i].d_un.d_val);
    }
  }

  if (dh->name==0) {	/* programs can have a LD_RUN_PATH */
    for(i=0;_dynamic[i].d_tag;i++) {
      if (_dynamic[i].d_tag==DT_RPATH) {
	register char *rpath=dh->dyn_str_tab+_dynamic[i].d_un.d_val;
	_dl_search_rpath=rpath;
	DEBUG("_dl_dyn_scan have runpath: %s\n",rpath);
      }
    }
  }

  DEBUG("_dl_dyn_scan post dynamic scan %08lx\n",(unsigned long)dh);

  if ((tmp=_dlsym(dh,"_GLOBAL_OFFSET_TABLE_"))) {
    dh->got=tmp;
    DEBUG("_dl_dyn_scan found a GOT @ %08lx\n",tmp);
    /* GOT */
    tmp[0]+=(unsigned long)dh->mem_base;	/* reloc dynamic pointer */
    tmp[1] =(unsigned long)dh;
    tmp[2] =(unsigned long)(_dl_jump);		/* sysdep jump to do_resolve */
  }
  else {
    _dl_error = 2;
    return 0;
  }

  DEBUG("_dl_dyn_scan pre load depending libraries %08lx\n",(unsigned long)dh);
  /* load depending libs */
#if 0	/* QUEUE */
  for(i=0;_dynamic[i].d_tag;i++) {
    if (_dynamic[i].d_tag==DT_NEEDED) {
      char *lib_name=dh->dyn_str_tab+_dynamic[i].d_un.d_val;
      DEBUG("_dl_dyn_scan needed for this lib: %s\n",lib_name);
      _dl_queue_lib(lib_name,dh->flag_global);
    }
  }
  if (_dl_open_dep()) {
    _dl_error = 0;
    return 0;
  }
#endif

  DEBUG("_dl_dyn_scan post load depending libraries, pre resolve %08lx\n",(unsigned long)dh);

  /* relocation */
  if (rel) {
    DEBUG("_dl_dyn_scan try to relocate some values\n");

#if 1
    if (_dl_relocate(dh,rel,relsize/relent)) return 0;
#else
    for (i=0;i<(relsize/relent);++i) {
      _dl_sys_write(2,"reloc\n",6);
      if (_dl_apply_relocate(dh,rel+i)) {
	_dl_error=3;
	return 0;
      }
    }
#endif
  }

  /* do PTL / GOT relocation */
  if (plt_rel) {
    _dl_rel_t *tmp,*max=((void*)plt_rel)+plt_relsz;
    DEBUG("_dl_dyn_scan rel plt/got\n");
    for(tmp=plt_rel;tmp<max;(char*)tmp=((char*)tmp)+sizeof(_dl_rel_t)) {
      if (dh->flag_global&RTLD_NOW) {
	unsigned long sym=(unsigned long)_dl_sym(dh,ELF_R_SYM(tmp->r_info));
	if (sym) *((unsigned long*)(dh->mem_base+tmp->r_offset))=sym;
	else {
	  _dl_error = 3;
	  return 0;
	}
      }
      else
	_DL_REL_PLT(dh->mem_base,tmp);
    }
  }

  DEBUG("_dl_dyn_scan post resolve, pre init %08lx\n",(unsigned long)dh);
  if (init) init();
  DEBUG("_dl_dyn_scan post init %08lx\n",(unsigned long)dh);

  return dh;
}


/* ELF AUX parser */
static void _dl_elfaux(register unsigned long*ui) {
  register struct elf_aux {
    unsigned long type;
    unsigned long val;
  } *ea;

  Elf_Phdr *ph=0;
  unsigned long ph_size;
  unsigned long ph_num;
  unsigned long pg_size;

  while (*ui) ++ui;
  /* now *ui points to the tailing NULL-pointer of the envirioment */

  /* walk the elf_aux table */
  for (ea=(struct elf_aux*)(ui+1); ea->type; ++ea) {
    switch (ea->type) {
    case AT_EXECFD:	/* 2 */
    case AT_NOTELF:	/* 10 */
      /* DIE! DIE! DIE! */
      _dl_sys_exit(42);
      break;

    case AT_PHDR:	/* 3 */
      ph=(Elf_Phdr*)ea->val;
      DEBUG("program header @ %08lx\n",(long)ph);
      break;
    case AT_PHENT:	/* 4 */
      ph_size=ea->val;
      DEBUG("program header size %08lx\n",ph_size);
      break;
    case AT_PHNUM:	/* 5 */
      ph_num=ea->val;
      DEBUG("program header # %ld\n",ph_num);
      break;

    case AT_PAGESZ:	/* 6 */
      pg_size=ea->val;
      DEBUG("page size %ld\n",pg_size);
      break;

    case AT_BASE:	/* 7 */
      loadaddr=ea->val;
      DEBUG("interpreter base: %08x\n",ea->val);
      break;

#if 0
    case AT_FLAGS:	/* 8 */
      DEBUG("flags %08x\n",ea->val);
      break;
#endif

    case AT_ENTRY:	/* 9 */
      prog_entry=ea->val;
      DEBUG("start program  @ %08lx\n",prog_entry);
      break;

    case AT_UID:	/* 11 */
      at_uid=ea->val;
      DEBUG(" UID: %d\n",ea->val);
      break;
    case AT_EUID:	/* 12 */
      at_euid=ea->val;
      DEBUG("EUID: %d\n",ea->val);
      break;
    case AT_GID:	/* 13 */
      at_gid=ea->val;
      DEBUG(" GID: %d\n",ea->val);
      break;
    case AT_EGID:	/* 14 */
      at_egid=ea->val;
      DEBUG("EGID: %d\n",ea->val);
      break;

#if 0
    case AT_PLATFORM:	/* 15 */
      DEBUG("CPU: %s\n",ea->val);
      break;
    case AT_HWCAP:	/* 16 */
      DEBUG("CPU capabilities: %08x\n",ea->val);
      break;
    case AT_CLKTCK:	/* 17 */
      DEBUG("CLK per sec %d\n", ea->val);
      break;
    case AT_FPUCW:	/* 18 */
      DEBUG("FPU control word %d\n", ea->val);
      break;
#endif

    default:
    }
  }
}


/* start of libdl dynamic linker */
static unsigned long _dl_main(int argc,char*argv[],char*envp[],unsigned long _dynamic) {
  unsigned long*got;
  struct _dl_handle my_dh;

  if (0) _dl_main(argc,argv,envp,_dynamic); /* TRICK: no warning */

  /* prepare to bootstarp the relocations */
  got=get_got();
  _dl_environ=envp;

  /* run elf_aux (kernel provided misc data) */
  _dl_elfaux((unsigned long*)envp);

  if (loadaddr==0) {
    pf("\ndiet libdl.so dynamic linker can't be started as a program !\n\n SORRY...\n\n");
    return (unsigned long)_DIE_;
  }

  memset(&my_dh,0,sizeof(my_dh));
  my_dh.mem_base=(char*)loadaddr;
  my_dh.name="libdl.so";
  my_dh.lnk_count=1;

  got[1]=0;			/* NOT YET (my_dh) */
  got[2]=(unsigned long)_DIE_;	/* NO lazy symbol resolver as long as we are not ready */

  pf("_dl_main: pre scan\n");
  /* bootstrap relocation */
  if (_dl_dyn_scan(&my_dh,(Elf_Dyn*)_dynamic)==0) {
    pf("error will dyn_scan myself\n");
    return (unsigned long)_DIE_;
  }
  pf("_dl_main: post scan\n");

  /* now we are save to use anything :) (hopefully) */

  fini_entry=tt_fini;

  /* now start the program */
  pf("now jump to program entrypoint\n");
  return prog_entry;
}

#endif
