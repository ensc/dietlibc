#include "dietfeatures.h"
#include "ppc64-features.h"

#if defined(_CALL_ELF) && _CALL_ELF == 2
	.section ".text"
	.globl	_start
	.type	_start,@function
_start:
	addis	2,12,.TOC.-_start@ha
	addi	2,2,.TOC.-_start@l
.localentry	_start,.-_start
#else
	.section ".text"
	.align	2
	.globl	._start
	.type	._start,@function
	.globl	_start
	.section ".opd","aw"
_start:
	.quad	._start
	.quad	.TOC.@tocbase, 0
	.previous
._start:

/*
	.section ".text"
	.align	2
	.globl	._start
	.type	._start,@function
	.globl	_start
	.section ".opd","aw"
_start:
	.quad	._start
	.quad	.TOC.@tocbase, 0
	.previous
._start:
*/
#endif
 /* Save the stack pointer, in case we're statically linked under Linux.  */
	mr	9,1
 /* Set up an initial stack frame, and clear the LR.  */
	clrrdi	1,1,4
	li	0,0
	stdu	1,-128(1)
	mtlr	0
	std	0,0(1)

 /* r9 contains the initial stack pointer
    argc = (r9)
    argv = (r9+4)
    envp = argv+(argc+1)*4 */

	lis	14,__libc_stack_end@highesta
	ori	14,14,__libc_stack_end@highera
	sldi    14,14,32
	oris	14,14,__libc_stack_end@ha
	std	9,__libc_stack_end@l(14)

	ldu	3,0(9)	/* argc */
	addi	4,9,8	/* argv */
	add	5,0,3	/* argc... */
	addi	5,5,1	/* argc+1...*/
	slwi	5,5,3	/* (argc+1)*8 */
	add	5,5,4	/* argv+(argc+1)*8 */

	lis	14,environ@highesta
	ori	14,14,environ@highera
	sldi    14,14,32
	oris	14,14,environ@ha
	std	5,environ@l(14)

#ifdef WANT_ELFINFO
1:	ldu	15,0(5)
	addi	5, 5, 8
	cmpdi	15,0
	bne	1b

	std	5,__elfinfo@l(14)
#endif

#ifdef WANT_DYNAMIC
/* #warning dynamic */
	mr	6,7
	/* This has not been tested on ppc64le yet... */
	bl	FN_NAME_DOTIFY(_dyn_start)
#else
/* #warning static */
	bl	FN_NAME_DOTIFY(CALL_IN_STARTCODE)
#endif
	/* Don't use the dotted version here, because __libc_exit is
	 * defined as an alias in the C code, so only the weak exit
	 * (but not .exit) will be overridden by it. Fortunately,
	 * calling the non-dotted symbols also works. This is required
	 * for atexit (and stdio's flush() at exit) to work. */
	b	exit

.LT_start:
	.long	0
	.quad	0x000c000000000000 | 0x0000200000000000 | 0x0000004000000000
	.long	.LT_start-FN_NAME_DOTIFY(_start)
	.short	.LT_start_name_end-.LT_start_name_start
.LT_start_name_start:
	.ascii "_start"
.LT_start_name_end:
	.align 2
	.size FN_NAME_DOTIFY(_start),.-FN_NAME_DOTIFY(_start)

/* Define a symbol for the first piece of initialized data.  */
	.section ".data"
__data_start:

/* this is needed for libgcc_eh.a - see gcc-3.4/gcc/config/rs6000/linux-unwind.h */
	.weak	__libc_stack_end
	.lcomm	__libc_stack_end,8,8
	.type	__libc_stack_end,@object

