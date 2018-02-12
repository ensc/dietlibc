#include "dietfeatures.h"
.text
.global strlen
.type strlen,@function
strlen:
	/*
	The slow part about strlen is accessing memory.
	Reading 1 byte is just as slow as reading 16 bytes.
	So we want to read 16 bytes at a time. That could technically
	read bytes behind the end of the string, which, if we cross a
	page boundary, could lead to a program crash. So we align our
	16-byte blocks on multiples of 16-byte offsets. That way we
	never cross a page boundary, however we could read before the
	buffer, which would cause a tool like valgrind to complain.
	So we optionally detect valgrind, and fall back to the slow
	path when it is there.
	*/

#ifdef WANT_VALGRIND_SUPPORT
#ifdef __PIC__
	movq	__valgrind@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	test	%eax, %eax
#else
	xor %eax,%eax
	or __valgrind(%rip),%eax
#endif
	jnz .Lvalgrind
#endif
        mov %rdi,%rax
        mov %edi,%ecx
        pxor %xmm0,%xmm0
        and $15,%ecx	/* ecx = how many bytes too far back alignment moved us */
        and $-16,%rax	/* rax = pointer after alignment */
        movdqa (%rax),%xmm1	/* read one vector */
        pcmpeqb %xmm0,%xmm1	/* cmp to all-0 vector */
        pmovmskb %xmm1,%edx	/* if any of the bits in edx are 1, we
				   had a 0 byte in the first vector */
        shr %cl,%edx		/* ignore 0 bytes in first n bytes (when
				   alignment moved us n bytes too far back */
        shl %cl,%edx
        test %edx,%edx
        jnz .L2
.L1:
        lea 16(%rax),%rax
        movdqa (%rax),%xmm1
        pcmpeqb %xmm0,%xmm1
        pmovmskb %xmm1,%edx
        test %edx,%edx
        jz .L1
.L2:
        sub %rax,%rdi
        bsf %edx,%eax
        sub %rdi,%rax
        ret
#ifdef WANT_VALGRIND_SUPPORT
.Lvalgrind:
	mov %rdi,%rax
.L3:
	cmpb $0,(%rax)
	jz .L4
	inc %rax
	jmp .L3
.L4:
	sub %rdi,%rax
	ret
#endif

.Lhere:
        .size    strlen,.Lhere-strlen

	.section	.note.GNU-stack,"",@progbits
