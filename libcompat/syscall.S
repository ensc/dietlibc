#include "dietfeatures.h"

#if defined(__i386__) && defined(__DYN_LIB)
#include "i386/PIC.h"
#undef WANT_THREADSAFE
#define WANT_THREADSAFE
#endif

.text
.globl syscall
#ifdef __arm__
.type syscall,function
#else
.type syscall,@function
#endif

syscall:
#ifdef __i386__
	movl %esp, %eax
	pushl %edi
	pushl %esi
	pushl %ebx
	pushl %ebp
	movl 0x08(%eax), %ebx
	movl 0x0c(%eax), %ecx
	movl 0x10(%eax), %edx
	movl 0x14(%eax), %esi
	movl 0x18(%eax), %edi
	movl 0x1c(%eax), %ebp
	movl 0x04(%eax), %eax
	int  $0x80
	cmpl $-255, %eax
	jbe .Lret
	negl %eax
#ifdef WANT_THREADSAFE
	movl %eax, %edx
#ifdef __DYN_LIB
	PIC_INIT
	call __errno_location@PLT
#else
	call __errno_location
#endif
	movl %edx, (%eax)
	xorl %eax, %eax
	decl %eax
#else
	mov %eax, errno
	sbb %eax, %eax
#endif
.Lret:
	pop %ebp
	pop %ebx
	pop %esi
	pop %edi
	ret
#elif (defined(__hppa__))
	copy %r26, %r20
	copy %r25, %r26
	copy %r24, %r25
	copy %r23, %r24
	ldw  -0x34(%sr0, %sp), %r23
	ldw  -0x38(%sr0, %sp), %r22
	be,l 0x100(%sr2, %r0), %sr0, %r31
	ldw  -0x3c(%sr0, %sp), %r21
	ldi -0x100, %r1
	cmpb,<<=,n %r1, %ret0, __error_unified_syscall
.Lret:
	bv,n %r0(%rp)
#elif (defined(__ia64__))
	mov r15=r32
	mov r32=r33
	mov r33=r34
	mov r34=r35
	mov r35=r36
	mov r36=r37
	break.i 0x100000
	cmp.eq p6,p0=-1,r10
(p6)	br __error_unified_syscall
	br.ret.sptk.few rp
#endif
.size	syscall, . - syscall
