#include "dietfeatures.h"
#include "syscalls.h"
#include "aarch64-features.h"

/* FIXME: dynamic library support: x0 contains ldso's _fini that needs
 *        to be registered with atexit... */

FUNC_START	_start
	mov	x29, #0			/* clear the frame pointer */
	mov	x30, #0			/* clear the link register */
	ldr	x0, [sp]		/* argc */
	add	x1, sp, #8		/* argv */
#ifdef __DYN_LIB
	adrp	x16, :got:environ
	ldr	x16, [x16, #:got_lo12:environ]
#else
	ldr	x16, =environ
#endif
	lsl	x9, x0, #3		/* argc * sizeof(void *) */
	add	x2, x1, x9		/* &argv[argc] */
	add	x2, x2, #8		/* envp */
	str	x2, [x16]		/* environ = envp */
	bl	CALL_IN_STARTCODE

/*
 * The exit status from main() is already in x0.
 * We need to branch to 'exit' in case we have linked with 'atexit'.
 */
	bl	exit
FUNC_END	_start

FUNC_START	_exit
FUNC_START_WEAK	exit
	mov	x8, #__NR_exit
	svc	#0			/* never returns. */

FUNC_END	exit
FUNC_END	_exit

.section .ctors,"aw"
.global __CTOR_LIST__
__CTOR_LIST__:

.section .dtors,"aw"
.global __DTOR_LIST__
__DTOR_LIST__:

.section .eh_frame,"aw"
.global __EH_FRAME_BEGIN__
__EH_FRAME_BEGIN__:
