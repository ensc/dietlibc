#include "dietfeatures.h"

.text
.global _start
.type _start,@function
_start:
 /* Save the stack pointer, in case we're statically linked under Linux.  */
	mr	9,1
 /* Set up an initial stack frame, and clear the LR.  */
	clrrwi	1,1,4
	li	0,0
	stwu	1,-16(1)
	mtlr	0
	stw	0,0(1)

 /* r9 contains the initial stack pointer
    argc = (r9)
    argv = (r9+4)
    envp = argv+(argc+1)*4 */

	lis	14,__libc_stack_end@ha
	stw	9,__libc_stack_end@l(14)

	lwzu	3,0(9)	/* argc */
	addi	4,9,4	/* argv */
	add	5,0,3	/* argc... */
	addi	5,5,1	/* argc+1...*/
	slwi	5,5,2	/* (argc+1)*4 */
	add	5,5,4	/* argv+(argc+1)*4 */

	lis	14,environ@ha
	stw	5,environ@l(14)

#ifdef WANT_ELFINFO
1:	lwzu	15,0(5)
	addi	5, 5, 4
	cmpwi	15,0
	bne	1b

	stw	5,__elfinfo@l(14)
#endif

#ifdef WANT_CTOR
	mr	6,7
#endif
	bl	CALL_IN_STARTCODE
	b	exit
.size _start,.-_start


/* Define a symbol for the first piece of initialized data.  */
	.section ".data"
__data_start:

/* this is needed for libgcc_eh.a - see gcc-3.4/gcc/config/rs6000/linux-unwind.h */
	.weak	__libc_stack_end
	.lcomm	__libc_stack_end,4,4
	.type	__libc_stack_end,@object

.section .ctors,"aw"
.global __CTOR_LIST__
__CTOR_LIST__:

.section .dtors,"aw"
.global __DTOR_LIST__
__DTOR_LIST__:

.section .eh_frame,"aw"
.global __EH_FRAME_BEGIN__
__EH_FRAME_BEGIN__:
