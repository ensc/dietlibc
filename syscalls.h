#ifdef __ASSEMBLER__

#include <asm/unistd.h>

#else

#include <linux/unistd.h>

#endif

#ifdef __i386__

#define syscall_weak(name,wsym,sym) \
.text; \
.weak wsym; \
wsym: ; \
.global sym; \
sym: \
	movb $__NR_##name,%al; \
	jmp __unified_syscall

#define syscall(name,sym) \
.text; \
.global sym; \
sym: \
	movb $__NR_##name,%al; \
	jmp __unified_syscall

#endif

#ifdef __sparc__

#define syscall_weak(name,wsym,sym) \
.text; \
.weak wsym; \
wsym: ; \
.global sym; \
sym: \
	b __unified_syscall; \
	mov __NR_##name, %g1

#define syscall(name,sym) \
.text; \
.global sym; \
sym: \
	b __unified_syscall; \
	mov __NR_##name, %g1

#endif

#ifdef __powerpc__

#define syscall_weak(name,wsym,sym) \
.text; \
.weak wsym; \
wsym: ; \
.global sym; \
sym: \
	li	0,__NR_##name; \
	b __unified_syscall

#define syscall(name,sym) \
.text; \
.global sym; \
sym: \
	li	0,__NR_##name; \
	b __unified_syscall

#endif


#ifdef __mips__

#define syscall_weak(name,wsym,sym) \
.text; \
.weak wsym; \
wsym: ; \
.global sym; \
.ent sym; \
sym: \
	li	$2,__NR_##name; \
	la	$25,__unified_syscall; \
	jr	$25; \
.end sym

#define syscall(name,sym) \
.text; \
.global sym; \
.ent sym; \
sym: \
	li	$2,__NR_##name; \
	la	$25,__unified_syscall; \
	jr	$25; \
.end sym

#endif

#ifdef __arm__

#define syscall_weak(name,wsym,sym) \
.text; \
.weak wsym; \
wsym: ; \
.global sym; \
sym: \
	swi	__NR_##name; \
	b	__unified_syscall

#define syscall(name,sym) \
.text; \
.global sym; \
sym: \
	swi	__NR_##name; \
	b	__unified_syscall

#endif


#ifdef __alpha__

#define syscall_weak(name,wsym,sym) \
.text ; \
.align 2 ; \
.weak wsym; \
.type wsym,@function ; \
wsym: ; \
.global sym ; \
.type sym,@function ; \
sym: ; \
        lda     $0, __NR_##name($31) ; \
        br      __unified_syscall

#define syscall(name,sym) \
.text ; \
.align 2 ; \
.global sym ; \
.type sym,@function ; \
sym: ; \
        lda     $0, __NR_##name($31) ; \
        br      __unified_syscall

#endif
