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
static void _dl_sys_exit(int);

#ifdef __i386__

asm(".text
.global _start
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
#	ret

# test / debug code :)
_dl_sys_exit:
	movl	$1,%eax
	popl	%ebx
	int	$0x80
	hlt

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
.global _start
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
};

const char *dlerror(void)
{
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


/* strcmp.c */

static int _dl_lib_strcmp(register const unsigned char *s,register const unsigned char *t) {
  register char x;
  for (;;) {
    x = *s; if (x != *t) break; if (!x) break; ++s; ++t;
  }
  return ((int)(unsigned int)x) - ((int)(unsigned int)*t);
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
    DEBUG("_dl_sym_search: searching: %08lx %08lx\n",(long)tmp, (long)dh);
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


/* lazy function resolver */
static unsigned long do_resolve(struct _dl_handle *dh, unsigned long off) {
  _dl_rel_t *tmp = ((void*)dh->plt_rel)+off;
  int sym=ELF_R_SYM(tmp->r_info);
  register unsigned long sym_val;

  if (0) sym_val=(unsigned long)do_resolve; /* TRICK: no warning */

  /* modify GOT for REAL symbol */
  //sym_val=((unsigned long)(tmp_dl->mem_base+tmp_dl->dyn_sym_tab[sym].st_value));
  sym_val=(unsigned long)_dl_sym(dh,sym);
  *((unsigned long*)(dh->mem_base+tmp->r_offset))=sym_val;

  /* JUMP (arg sysdep...) */
  if (sym_val) return sym_val;
  /* can't find symbol */
  return (unsigned long)_DIE_;
}

/* bootstarp code */
static void bootstrap(struct _dl_handle*dh,Elf_Dyn*_dynamic,unsigned long load_addr) {
  int i;
  for(i=0;_dynamic[i].d_tag;i++) {
    switch (_dynamic[i].d_tag) {
    case DT_HASH:
    case DT_STRTAB:
    case DT_JMPREL:
      _dynamic[i].d_un.d_ptr+=load_addr;
      break;
    default:
      break;
    }
  }
}
//void _dl_dyn_scan(struct _dl_handle*dh,Elf_Dyn*_dynamic,unsigned long load_addr) __attribute__((alias("bootstrap")));


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
  load_addr=_dynamic-got[0];

  got[0]=_dynamic;		/* write relocated address of _DYNAMIC */
  got[1]=0;			/* NOT YET (my_dh) */
  got[2]=(unsigned long)(_DIE_);/* NO lazy symbol relocation as long as we are not ready */

  /* bootstrap relocation */
  bootstrap(&my_dh,(Elf_Dyn*)_dynamic,load_addr);

  got[2]=(unsigned long)(_dl_jump);/* NO lazy symbol relocation as long as we are not ready */
  /* now we are save to use anything :) */

  fini_entry=0;
  return load_addr;
}

#endif
