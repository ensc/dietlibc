#ifdef __i386__

.text
.globl ldso_start
ldso_start:
	movl %esp, %ebp			/* save stack pointer */
	movl (%ebp), %ecx		/* %ecx = argc */
	leal 4(%ebp), %esi		/* %esi = argv */
	leal 4(%esi,%ecx,4), %eax	/* eax = esi+4*ecx+4 = envp */

	/* call ldso main */
	pushl %eax
	pushl %esi
	pushl %ecx
	movl %eax, environ
	call main
	movl %ebp, %esp		/* restore stack */
	push %eax		/* push start entry of program */

	xorl %eax,%eax		/* clear registers */
	xorl %ebx,%ebx		/* as if kernel starts s static program */
	xorl %ecx,%ecx
	xorl %esi,%esi
	xorl %edi,%edi
	xorl %ebp,%ebp

	movl fini_entry, %edx	/* the dynamic fini for all *.so loaded */
	ret			/* jump to entry point of program */

#endif

.data

.section ".bss"
.align 8

.global errno
errno:
	.long 0

.global environ
environ:
.global __environ
__environ:
	.long 0

.global fini_entry
fini_entry:
	.long 0
