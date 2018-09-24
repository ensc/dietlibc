#include <dietfeatures.h>
#include "syscalls.h"
#include "ppc64-features.h"

	.text
#if defined(_CALL_ELF) && _CALL_ELF == 2
exit:
_exit:
_Exit:
	addis	2,12,.TOC.-exit@ha
	addi	2,2,.TOC.-exit@l
.localentry	exit,.-exit
.localentry	_exit,.-_exit
.localentry	_Exit,.-_Exit
#else
.exit:
	.weak	.exit
._exit:
._Exit:
#endif
	li	0,__NR_exit
	.global __unified_syscall
__unified_syscall:
	sc
	bnslr+

	.global error_unified_syscall
error_unified_syscall:

#ifdef WANT_THREAD_SAFE
	stdu	1,-128(1)
	mflr	0
	std	0,20(1)
	std	3,12(1)
	bl	FN_NAME_DOTIFY(__errno_location)
	ld	0,12(1)
	stw	0,0(3)
	ld	0,20(1)
	mtlr	0
	addi	1,1,128
#else
	/* FIXME: check on ppc64el */
	lis	9,errno@highesta
	ori	9,9,errno@highera
	sldi    9,9,32
	oris	9,9,errno@ha
	stw	3,errno@l(9)
#endif
	li	3,-1

/* here we go and "reuse" the return for weak-void functions */
//#include "dietuglyweaks.h"

/* On ppc64el: Since only a return is reused, and we don't need r2 to
 * be available, it's perfectly legitimate to have global entry point
 * and local entry point of these functions to coincide, so we don't
 * need either explicit code to initialize r2 nor a .localentry marker.
 */
FN_NAME_DOTIFY(__thread_doexit):
FN_NAME_DOTIFY(__fflush_stdin):
FN_NAME_DOTIFY(__fflush_stdout):
FN_NAME_DOTIFY(__fflush_stderr):
FN_NAME_DOTIFY(flockfile):
FN_NAME_DOTIFY(funlockfile):
FN_NAME_DOTIFY(__nop):
FN_NAME_DOTIFY(__you_tried_to_link_a_dietlibc_object_against_glibc):

	blr
	.long 0
	.byte 0,0,0,1,128,0,0,0
	.size	FN_NAME_DOTIFY(_exit),.-FN_NAME_DOTIFY(_exit)
	.size	FN_NAME_DOTIFY(_Exit),.-FN_NAME_DOTIFY(_Exit)
	.size	FN_NAME_DOTIFY(exit),.-FN_NAME_DOTIFY(exit)

diet_proto_weak(__thread_doexit)
diet_proto_weak(__fflush_stdin)
diet_proto_weak(__fflush_stdout)
diet_proto_weak(__fflush_stderr)
diet_proto_weak(flockfile)
diet_proto_weak(funlockfile)
diet_proto_weak(__nop)
diet_proto_weak(__you_tried_to_link_a_dietlibc_object_against_glibc)
diet_proto_weak(exit)
diet_proto(_exit)
diet_proto(_Exit)
