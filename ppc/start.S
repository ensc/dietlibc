#include "start.h"

#ifdef __powerpc__

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

 /* r9 enthält den initialen Stackpointer.
    argc = (r9)
    argv = (r9+4)
    envp = argv+(argc+1)*4 */

	lwzu	3,0(9)	/* argc */
	addi	4,9,4	/* argv */
	add	5,0,3	/* argc... */
	addi	5,5,1	/* argc+1...*/
	slwi	5,5,2	/* (argc+1)*4 */
	add	5,5,4	/* argv+(argc+1)*4 */

	lis	6,environ@ha
	stw	5,environ@l(6)

	bl	main
	b	exit
.size _start,.-_start


/* Define a symbol for the first piece of initialized data.  */
	.section ".data"
__data_start:


#endif

