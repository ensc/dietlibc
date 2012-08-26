#include "dietfeatures.h"

.text
.global _start
_start:
	basr	%r13,%r0
	ahi	%r13,[.Laddr-.]

#ifdef WANT_DYNAMIC
	lr	%r5,%r14		/* save dynamic ld.so _fini */
#endif

	l	%r2,0(%r15)		/* argc */
	la	%r3,4(%r15)		/* argv */

/* envp = argv + ( (argc+1)*4 ) */
	lr	%r4,%r2
	sll	%r4,2
	la	%r4,8(%r4,%r15)

/* align the stack to a double word boundary */
	lhi     %r0,-8
	nr      %r15,%r0

/* Setup a stack frame and a parameter area */
	ahi     %r15,-104               # make room on stack
	xc      0(4,%r15),0(%r15)       # clear back-chain

/* environ = envp */
	l	%r1,8(%r13)
	st	%r4,0(%r1)

#ifdef WANT_ELFINFO
	lhi	%r6, -4
1:	ahi	%r4, 4		# increment envp
	l	%r12, 0(%r6,%r4)	# load envp[-1] into %r12
	or	%r12, %r12	# test %r12 for NULL
	jne	1b

	ahi	%r1, 4
	st	%r4,0(%r1)
#endif

/* call main or _dyn_start */
	l	%r1,0(%r13)
	basr	%r14,%r1

/* call exit */
	l	%r1,4(%r13)
	basr	%r14,%r1

/* DIE! */
	.word	0

.align 4
.Laddr:
#ifdef WANT_DYNAMIC
	.long	_dyn_start
#else
	.long	CALL_IN_STARTCODE
#endif
	.long	exit
	.long	environ

.Lstart:
	.size	_start,.Lstart-_start

