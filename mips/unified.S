#include <dietfeatures.h>

	.text
	.align	2
	.globl	__unified_syscall
	.ent	__unified_syscall
__unified_syscall:
	syscall
	bne	$7, $0, .Lerror

/* here we go an "reused" the return for weak-void functions */
#include "dietuglyweaks.h"

	jr	$31
	.end	__unified_syscall

	.global error_unified_syscall
	.ent	error_unified_syscall
error_unified_syscall:
.Lerror:
#ifdef WANT_THREAD_SAFE
	addiu	$sp, $sp, -24

	sw	$31, 0($sp)
	sw	$2,  4($sp)

	la	$25, __errno_location
	jalr	$25

	lw	$8,  4($sp)
	lw	$31, 0($sp)
	sw	$8,  0($2)
	li	$2,  -1
	addiu	$sp, $sp, 24
	jr	$31
#else
	sw	$2,  errno
	li	$2,  -1
	jr	$31
#endif
	.end	error_unified_syscall
