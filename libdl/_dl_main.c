/*
 * the is the dietlibc libdl dynamic-linker
 */
#include "_dl_int.h"

static void (*fini_entry)(void);

void _start(void);

#ifdef __i386__

asm(".text
.global _start
.type _start,@function
_start:
	movl	%esp, %ebp		# save stack
	movl	(%ebp), %ecx		# argc
	leal	4(%ebp), %esi		# argv
	leal	4(%esi,%ecx,4), %eax	# envp

# PIC code
	call	.L_pic
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx

# get load addr ( this generates TEXTREL :( )
	leal	0f@GOTOFF(%ebx), %edx
0:	subl	$., %edx

# put parameter on stack and call main
	pushl	%edx
	pushl	%eax
	pushl	%esi
	pushl	%ecx
	call	_dl_main

# restore stack
	movl	%ebp, %esp

# save program entry point
	pushl	%eax

# get fini pointer
	movl	fini_entry@GOTOFF(%ebx), %edx

# clear register like kernel
	xorl	%eax, %eax
	xorl	%ecx, %ecx
	xorl	%ebx, %ebx
	xorl	%ebp, %ebp
	xorl	%edi, %edi
	xorl	%esi, %esi

# jump to program entry point
#	ret

# test / debug code :)
_exit:
	movl	$1,%eax
	popl	%ebx
	int	$0x80
	hlt

# get Position In Code :)
.L_pic:	movl	(%esp), %ebx
	ret
");

static inline unsigned long* get_got(void) {
  register unsigned long *got asm ("%ebx");
  return got;
}

#elif __arm__

asm(".text
.global _start
_start:
	mov	r4, sp
	mov	fp, #0

	ldr	a1, [sp], #4
	mov	a2, sp
	add	a3, a2, a1, lsl #2
	add	a3, a3, #4

	ldr	sl, .L_got
1:	add	sl, pc, sl

	ldr	a4, .L_la
	mov	a4, pc

	bl	_dl_main

	mov	sp, r4
	mov	lr, a1
	ldr	a1, [pc, #.L_fe-(.+8)]
	mov	pc, lr

.L_got:	.long	_GLOBAL_OFFSET_TABLE_-(1b+8)
.L_la:	.long	.L_la(GOTOFF)
.L_fe:	.long	fini_entry(GOTOFF)
");

static inline unsigned long* get_got(void) {
  register unsigned long *got asm ("sl");
  return got;
}

#else
#error "arch not supported"
#endif

static unsigned long _dl_main(int argc,char*argv[],char*envp[],unsigned long load_addr) {
  if (0) _dl_main(argc,argv,envp,load_addr); /* TRICK: no warning */
  /* bootstrap relocation */
  {
    unsigned long*got=get_got();
    fini_entry=got;
  }

  /* now we are save to use anything :) */
  return load_addr;
}
