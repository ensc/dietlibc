#ifdef __OD_CLEAN_ROOM

/*
 * the is the dietlibc libdl dynamic-linker
 *
 * NEED to be compiled with -fPIC ...
 */
#include "_dl_int.h"

static void (*fini_entry)(void);

void _start(void);
void _dl_jump(void);
void _exit(int);

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
.global _exit
_exit:
	movl	$1,%eax
	popl	%ebx
	int	$0x80
	hlt

.global _dl_jump
.type	_dl_jump,@function
_dl_jump:
	pushl	%eax		# save register args...
	pushl	%ecx
	pushl	%edx

	push	20(%esp)	# 2. arg from plt
	push	20(%esp)	# 1. arg from plt
	call	do_rel
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

_exit:
	swi	#1			@ exit
	eor	lr, lr, lr		@ OR DIE !
	mov	pc, lr

.global _dl_jump
.type	_dl_jump,function
_dl_jump:
	stmdb	sp!, {r0, r1, r2, r3}	@ save arguments

	sub	r1, ip, lr		@ dyntab entry
	sub	r1, r1, #4
	add	r1, r1, r1

	ldr	r0, [lr, #-4]		@ dynlib handle

	bl	do_rel

	mov	r12, r0
	ldmia	sp!, {r0, r1, r2, r3, lr} @ restore arguments
	mov	pc, r12
");

static inline unsigned long* get_got(void) {
  register unsigned long *got asm ("sl");
  return got;
}

#else
#error "arch not supported"
#endif

/* exit ! */
static void _DIE_() { _exit(213); }

/* lazy function resolver */
static unsigned long do_rel(struct _dl_handle * tmp_dl, unsigned long off) {
  Elf_Rel *tmp = ((void*)tmp_dl->plt_rel)+off;
  int sym=ELF_R_SYM(tmp->r_info);
  register unsigned long sym_val;

  /* modify GOT for REAL symbol */
  //sym_val=((unsigned long)(tmp_dl->mem_base+tmp_dl->dyn_sym_tab[sym].st_value));
  sym_val=(unsigned long)_dl_sym(tmp_dl,sym);
  *((unsigned long*)(tmp_dl->mem_base+tmp->r_offset))=sym_val;

  /* JUMP (arg sysdep...) */
  if (sym_val) return sym_val;
  /* can't find symbol */
  return (unsigned long)_DIE_;
}

/* bootstarp code */
static void bootstrap(Elf_Dyn*_dynamic) {
}

/* start of libdl dynamic linker */
static unsigned long _dl_main(int argc,char*argv[],char*envp[],unsigned long _dynamic) {
  unsigned long*got;
  unsigned long load_addr;

  if (0) _dl_main(argc,argv,envp,load_addr); /* TRICK: no warning */

  /* prepare to bootstarp the relocations */
  got=get_got();

  /* first element of GOT points to _DYNAMIC (ELF convention)
   * _dynamic - (UNRELOCATED offset) == load address */
  load_addr=_dynamic-got[0];

  got[0]=_dynamic;		/* write relocated address of _DYNAMIC */
  got[1]=0;			/* NOT YET */
  got[2]=(unsigned long)(_DIE_);/* NO dynamic symbol relocation as long as we are not ready */

  /* bootstrap relocation */
  bootstrap((Elf_Dyn*)_dynamic);

  /* now we are save to use anything :) */

  return load_addr;
}

#endif
