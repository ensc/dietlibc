#include "start.h"

#ifdef __alpha__

.text
.align 2
	.set noreorder

.global __start
__start:
.global _start
_start:
	clr	$fp		/* clear frame pointer */

	br	$gp,.Lstart	/* set global pointer */
.Lstart:
	ldgp	$gp, 0($gp)

/* prepare to call main */
	ldl	$16, 0($sp)	/* argc */

	lda	$17, 8($sp)	/* argv */

	addq	$16,   1, $18	/* argp */
	s8addq	$18, $17, $18	/* (8*(argc+1))+argv -> argp  */

	stq	$18, environ

	mov	 $0, $21	/* mov v0(dynload) to a5 */

	jsr	$26, main
	mov	 $0, $16
	jsr	$26, exit	/* YES, CALL! for threads and atexit ! (+4 byte) */

#endif
