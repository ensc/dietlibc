#include <errno.h>
#include "syscalls.h"

.text
.weak clone
clone:
.type __clone,function
.global __clone
__clone:
	cmp	r0, #0		/* check for function pointer */
	cmpne	r1, #0		/* if function check for stack pointer */
	moveq	r0, #-EINVAL	/* if one is not available set errno value */
	beq	.Lerror		/* if error jump */

	sub	r1, r1, #8	/* reserve 2 words on child-stack */
	str	r3, [r1, #4]	/* function parameter */
	str	r0, [r1]	/* save function pointer */

	mov	r0, r2		/* move flags to r0 */
	swi	__NR_clone	/* syscall: clone */
	movs	r0, r0		/* set condition codes */
	blt	.Lerror		/* on error jump out */
	movne	pc, lr		/* return if "parent" */

	mov	fp, #0		/* clear the frame pointer */
	ldr	r0, [sp, #4]	/* load arg in argument register 0 */
	mov	lr, pc		/* save local addr as return addr */
	ldr	pc, [sp]	/* jmp to thread function */
	b	_exit		/* thread returned */

.Lerror:
	b	__unified_syscall	/* make error handling */
