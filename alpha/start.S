#include "dietfeatures.h"

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
	ldl	$16, 0($sp)	/* argc / a0 */

	lda	$17, 8($sp)	/* argv / a1 */

	addq	$16,   1, $18	/* argp / a2 */
	s8addq	$18, $17, $18	/* (8*(argc+1))+argv -> argp  */

	stq	$18, environ

#ifdef WANT_ELFINFO
#  warning "MAKE ME alpha ASSEMBLER!"
1:	ldq	$19, $18	; load *envp into $19
	addq	$18,  1,  $18	; increment *envp
	orr	$19, $19, $19
	jne	1b
	stq	$18, __elfinfo
#endif

#ifdef WANT_CTOR
/* in v0 ($0) is the ld.so _fini pointer */
	mov	 $0, $19	/* mov v0(dynload) to a3 */
#endif
	jsr	$26, CALL_IN_STARTCODE
	mov	 $0, $16
	jsr	$26, exit	/* YES, CALL! for threads and atexit ! (+4 byte) */

.section .ctors,"aw"
.global __CTOR_LIST__
__CTOR_LIST__:

.section .dtors,"aw"
.global __DTOR_LIST__
__DTOR_LIST__:

.section .eh_frame,"aw"
.global __EH_FRAME_BEGIN__
__EH_FRAME_BEGIN__:
