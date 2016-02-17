#include "dietfeatures.h"
.text
.global strlen
#ifdef __PIE__
.hidden strlen
#endif
.type strlen,@function
strlen:
#ifdef WANT_VALGRIND_SUPPORT
#ifdef __DYN_LIB
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
        and $15,%ecx
        and $-16,%rax
        movdqa (%rax),%xmm1
        pcmpeqb %xmm0,%xmm1
        pmovmskb %xmm1,%edx
        shr %cl,%edx
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
