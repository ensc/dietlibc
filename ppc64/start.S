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

	ldu	3,0(9)	/* argc */
	addi	4,9,8	/* argv */
	add	5,0,3	/* argc... */
	addi	5,5,1	/* argc+1...*/
	slwi	5,5,3	/* (argc+1)*4 */
	add	5,5,4	/* argv+(argc+1)*4 */

	lis	14,environ@ha
	std	5,environ@l(14)

#ifdef WANT_DYNAMIC
#warning dynamic
	mr	6,7
	bl	._dyn_start
#else
#warning static
	bl	.main
#endif
	b	.exit

.LT_start:
	.long	0
	.quad	0x000c000000000000 | 0x0000200000000000 | 0x0000004000000000
	.long	.LT_start-._start
	.short	.LT_start_name_end-.LT_start_name_start
.LT_start_name_start:
	.ascii "_start"
.LT_start_name_end:
	.align 2
	.size _start,.-_start

/* Define a symbol for the first piece of initialized data.  */
	.section ".data"
__data_start:

