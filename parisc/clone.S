#include <errno.h>
#include "syscalls.h"

.global __clone
__clone:
	cmpib,= 0,%r26,.Lerror	/* function pointer ? */
	ldi	-EINVAL,%r28
	cmpib,= 0,%r25,.Lerror	/* stack pointer ? */
	nop

	stwm	%r26, 64(sr0,%r25)	/* upgrowing stack ... */
	stw	%r23,-60(sr0,%r25)

	/* syscall */
	copy	%r24,%r26
	ble	0x100(%sr2,%r0)
	ldi	__NR_clone,%r20

	/* error ? */
	ldi	-4096,%r1
	comclr,>>= %r1,%r28,%r0
	b,n	.Lerror

	/* am I the child ? */
	comib,=,n 0,%r28,thread_start

	/* parent */
	bv	%r0(%r2)
	nop

.Lerror:/* there was an error */
	b	__error_unified_syscall
	sub	%r0,%r28,%r28	/* %r26 = -%r28 */

thread_start:
	ldw	-60(%sr0,%r30),%r26	/* load argument */
	ldw	-64(%sr0,%r30),%r22	/* function ptr */

	bl	$$dyncall,%r31		/* microcode call / is in libgcc */
	copy	%r31,%r2

	bl	_exit,%r2		/* exit thread */
	copy	%r28,%r26

	iitlbp	%r0,(%r0)		/* DIE ! DIE ! */
