#ifdef __OD_CLEAN_ROOM

/*
 * this is the dietlibc libdl & dynamic-linker
 *
 * NEED to be compiled with -fPIC ...
 */
#include "_dl_int.h"
#include "_dl_rel.h"

void _start(void);	/* entry of lib... */

static void (*fini_entry)(void);

#warning "Ignore all warnings about 'used but never defined' functions..."
static void _dl_jump(void);
static void _dl_sys_exit(int val);
static int _dl_sys_write(int fd,char*buf,unsigned long len);

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

# save program entry point
	pushl	%eax

# get fini pointer
	movl	fini_entry@GOTOFF(%ebx), %edx

# clear register like kernel
	xorl	%eax, %eax
	xorl	%ecx, %ecx
	xorl	%ebx, %ebx
	xorl	%ebp, %ebp
	xorl	%edi, %edi
	xorl	%esi, %esi

# jump to program entry point
	ret

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

static inline unsigned long get_base(void) {
  unsigned long ret;
  asm("leal	_start@GOTOFF(%%ebx),%0\n"
      "subl	_start@GOT(%%ebx),%0" : "=r"(ret) : : "cc");
  return ret;
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

static unsigned long get_base(void) {		/* no inline otherwise _start it too far away :( */
  extern void __dl_start asm ("_start");
  unsigned long gaddr = (unsigned long)&__dl_start;
  unsigned long raddr;
  asm ("adr %0, _start" : "=r"(raddr) );
  return raddr - gaddr;
}

#else
#error "libdl: arch not supported"
#endif


/* here do the code includes */

/* exit ! */
static void _DIE_() { _dl_sys_exit(213); }

/* strncpy */
static char *_dl_lib_strncpy(register char *s, register const char *t, register unsigned long n) {
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
static unsigned long _dl_lib_strlen(register const char *s) {
  register unsigned long i;
  if (!s) return 0;
  for (i=0; *s; ++s) ++i;
  return i;
}

/* strncat.c */
static char *_dl_lib_strncat(register char *s, register const char *t, unsigned long n) {
  char *dest=s;
  register char *max;
  s+=_dl_lib_strlen(s);

  if ((max=s+n-1)<=s) goto fini;

  for (;;) {
    if (!(*s = *t)) break; if (++s==max) break; ++t;
  }
  *s=0;
fini:
  return dest;
}

/* strcmp.c */
static int _dl_lib_strcmp(register const unsigned char *s,register const unsigned char *t) {
  register char x;
  for (;;) {
    x = *s; if (x != *t) break; if (!x) break; ++s; ++t;
  }
  return ((int)(unsigned int)x) - ((int)(unsigned int)*t);
}

/* memcpy.c */
static void* _dl_lib_memcpy(void* dst, const void* src, unsigned long count) {
  register char *d=dst;
  register const char *s=src;
  ++count;	/* this actually produces better code than using count-- */
  while (--count) {
    *d = *s;
    ++d; ++s;
  }
  return dst;
}

static void *memset(void *dst, int ch, unsigned long count) {
  register char *d=dst;
  ++count;
  while (--count) {
    *d=ch;
    ++d;
  }
  return dst;
}

#define memcpy(d,s,c) _dl_lib_memcpy(d,s,c)


/* dlerror.c */
static int _dl_error;
static const char* _dl_error_location;
static const char* _dl_error_data;

static struct _dl_err_msg {
  char *msg;
  int len;
} _dl_error_msg[] = {
#define MSG(n) { (n), sizeof((n)) }
  MSG("can't open: "),
  MSG("can't stat: "),
  MSG("shared object is not position independent: "),
  MSG("can't resolve all symbols in: "),
  MSG("can't find symbol: "),
  MSG("invalid relocation type in: "),
};

const char *dlerror(void) {
  static char buf[1024];
  register int len=sizeof(buf)-1;
  buf[0]=0;
  buf[len]=0;

  if (_dl_error>=(sizeof(_dl_error_msg)/sizeof(struct _dl_err_msg)))
    return "HAE ?!?";

  _dl_lib_strncpy(buf,_dl_error_location,len); len -= sizeof(_dl_error_location);
  _dl_lib_strncat(buf,": ",len); len-=2;
  _dl_lib_strncat(buf,_dl_error_msg[_dl_error].msg,len); len -= _dl_error_msg[_dl_error].len;
  _dl_lib_strncat(buf,_dl_error_data,len);

  _dl_error_data="";
  return buf;
}


static struct _dl_handle* _dl_root_handle;

static void _fini_run(struct _dl_handle * tmp) {
  if (tmp->fini) tmp->fini();
  if (tmp->next) _fini_run(tmp->next);
}
static void tt_fini(void) {
  DEBUG("dyn fini\n");
  _fini_run(_dl_root_handle);
}


/* dlsym.c + _dl_sym.c */
#include "elf_hash.h"

static void*_dlsym(void *handle, char *symbol) {
  void *sym=0;
  if (handle) {
    struct _dl_handle *dh=(struct _dl_handle*)handle;
    unsigned long  hash  = elf_hash(symbol);
    unsigned long  bhash = hash%HASH_BUCKET_LEN(dh->hash_tab);
    unsigned long* chain = HASH_CHAIN(dh->hash_tab);
    unsigned long  ind;
    char *name = dh->dyn_str_tab;

    ind=HASH_BUCKET(dh->hash_tab)[bhash];
    while(ind) {
      int ptr = dh->dyn_sym_tab[ind].st_name;
      if (_dl_lib_strcmp(name+ptr,symbol)==0) {
	if (dh->dyn_sym_tab[ind].st_value!=0) {
	  sym = (long*)(dh->mem_base+dh->dyn_sym_tab[ind].st_value);
	  break;	/* ok found ... */
	}
      }
      ind = chain[ind];
    }
    DEBUG("dlsym: symbol \"%s\" @ %08lx\n",symbol,(long)sym);
  }
  return sym;
}

void *dlsym(void *handle, char *symbol) {
  _dl_error_location = "dlsym";
  _dl_error_data = symbol;
  _dl_error = 4;
  return _dlsym(handle,symbol);
}

static void *_dl_sym_search(struct _dl_handle *dh, int symbol) {
  void *sym=0;
  struct _dl_handle *tmp;
  char *name=dh->dyn_str_tab+dh->dyn_sym_tab[symbol].st_name;
  DEBUG("_dl_sym_search: search for: %s\n",name);
  for (tmp=_dl_root_handle;tmp && (!sym);tmp=tmp->next) {
    if (tmp==dh) continue;
    DEBUG("_dl_sym_search: searching in %s\n",tmp->name);
    sym=_dlsym((void*)tmp,name);
    if (sym) DEBUG("_dl_sym_search: found: %s @ %08lx\n",name,(long)sym);
  }
  return sym;
}

static void *_dl_sym(struct _dl_handle *dh, int symbol) {
  void *sym=0;
  if (ELF_ST_TYPE(dh->dyn_sym_tab[symbol].st_shndx)!=0) {
    sym = dh->mem_base+dh->dyn_sym_tab[symbol].st_value;
  } else {
    sym = _dl_sym_search(dh,symbol);
  }
  DEBUG("_dl_sym %d -> %08lx\n",symbol,(long)ret);
  return sym;
}

#include "_dl_relocate.c"

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

    case DT_FINI:
      dh->fini = (void(*)(void))(dh->mem_base+_dynamic[i].d_un.d_val);
      DEBUG("_dl_dyn_scan have fini @ %08lx\n",(long)dh->fini);
      break;
    case DT_INIT:
      init = (void(*)(void))(dh->mem_base+_dynamic[i].d_un.d_val);
      DEBUG("_dl_dyn_scan have init @ %08lx\n",(long)dh->init);
      break;

    case DT_PLTGOT:
      dh->pltgot = (unsigned long*)(dh->mem_base+_dynamic[i].d_un.d_val);
      DEBUG("_dl_dyn_scan have plt got @ %08lx\n",(long)dh->pltgot);
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

    case DT_SONAME:
      if (dh->name) {
	dh->name = dh->dyn_str_tab+_dynamic[i].d_un.d_val;
	DEBUG("_dl_dyn_scan have soname: %s\n",dh->name);
      }
      break;

    case DT_PLTREL:
      if (_dynamic[i].d_un.d_val!=_DL_REL_T) {
	DEBUG("_dl_dyn_scan have incompatible relocation type\n");
	_dl_error = 5;
	return 0;
      }
      break;

    case DT_TEXTREL:
      DEBUG("_dl_dyn_scan found possible textrelocation -> %s is no compiled as a shared library\n",dh->name);
      _dl_error = 2;
      return 0;
      break;

    default:
      DEBUG("_dl_dyn_scan: unknown %d, %08lx\n",_dynamic[i].d_tag,_dynamic[i].d_un.d_val);
    }
  }

#if 0
  if (dh->name==0) {
    for(i=0;_dynamic[i].d_tag;i++) {
      if (_dynamic[i].d_tag==DT_RPATH) {
	char *rpath=dh->dyn_str_tab+_dynamic[i].d_un.d_val;
	_dl_set_rpath(rpath);
	DEBUG("_dl_dyn_scan have runpath: %s\n",rpath);
      }
    }
  }
#endif

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
#if 0
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

    for (i=0;i<(relsize/relent);++i) {
      _dl_sys_write(2,"reloc\n",6);
      if (_dl_apply_relocate(dh,rel+i)) {
	_dl_error=3;
	return 0;
      }
    }

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
static unsigned long _dl_elfaux(register unsigned long*ui) {
  register struct elf_aux {
    unsigned long type;
    unsigned long val;
  } *ea;
  unsigned long dyn_start=(unsigned long)_DIE_;

  Elf_Phdr *ph=0;
  unsigned long ph_size;
  unsigned long ph_num;
  unsigned long pg_size;


  while (*ui) ++ui;
  /* now *ui points to the tailing NULL-pointer of the envirioment */

  /* walk the elf_aux table */
  for (ea=(struct elf_aux*)(ui+1); ea->type; ++ea) {
    switch (ea->type) {
    case AT_EXECFD:
    case AT_NOTELF:
      _dl_sys_exit(42);
      break;

    case AT_PHDR:
      ph=(Elf_Phdr*)ea->val;
      DEBUG("program header @ %08lx\n",(long)ph);
      break;
    case AT_PHENT:
      ph_size=ea->val;
      DEBUG("program header size %08lx\n",ph_size);
      break;
    case AT_PHNUM:
      ph_num=ea->val;
      DEBUG("program header # %ld\n",ph_num);
      break;

#if 0
    case AT_BASE:
      DEBUG("base: %08x\n",ea->val);
      break;
    case AT_FLAGS:
      DEBUG("flags %08x\n",ea->val);
      break;

    case AT_UID:
      DEBUG(" UID: %d\n",ea->val);
      break;
    case AT_EUID:
      DEBUG("EUID: %d\n",ea->val);
      break;
    case AT_GID:
      DEBUG(" GID: %d\n",ea->val);
      break;
    case AT_EGID:
      DEBUG("EGID: %d\n",ea->val);
      break;
#endif

#if 1
    case AT_PAGESZ:
      pg_size=ea->val;
      DEBUG("page size %ld\n",pg_size);
      break;
#endif

    case AT_ENTRY:
      if (ea->val!=(unsigned long)_start) {
	dyn_start=ea->val;
	_dl_sys_write(1,"start\n",6);
	DEBUG("start program  @ %08lx\n",(long)dyn_start);
      }
      break;

#if 0
    case AT_PLATFORM:
      DEBUG("CPU: %s\n",ea->val);
      break;
    case AT_HWCAP:
      DEBUG("CPU capabilities: %08x\n",ea->val);
      break;
    case AT_CLKTCK:
      DEBUG("CLK per sec %d\n", ea->val);
      break;
#endif

    default:
    }
  }

  return dyn_start;
}


/* start of libdl dynamic linker */
static unsigned long _dl_main(int argc,char*argv[],char*envp[],unsigned long _dynamic) {
  unsigned long*got;
  unsigned long load_addr;
  struct _dl_handle my_dh;

  if (0) _dl_main(argc,argv,envp,load_addr); /* TRICK: no warning */

  /* prepare to bootstarp the relocations */
  got=get_got();

  /* first element of GOT points to _DYNAMIC (ELF convention)
   * _dynamic - (UNRELOCATED offset) == load address */
  /* load_addr=_dynamic-got[0]; */
  load_addr=get_base();

  memset(&my_dh,0,sizeof(my_dh));
  my_dh.mem_base=(char*)load_addr;
  my_dh.name="libdl.so";
  my_dh.lnk_count=1;

//  got[0]=_dynamic;		/* write relocated address of _DYNAMIC */
  got[1]=0;			/* NOT YET (my_dh) */
  got[2]=(unsigned long)(_DIE_);/* NO lazy symbol relocation as long as we are not ready */

  /* bootstrap relocation */
  if (_dl_dyn_scan(&my_dh,(Elf_Dyn*)_dynamic)==0) {
    _dl_sys_write(2,"error will dyn_scan myself\n",27);
    return (unsigned long)_DIE_;
  }

  /* now we are save to use anything :) (hopefuly) */

  fini_entry=tt_fini;
  return _dl_elfaux((unsigned long*)envp);
}

#endif
