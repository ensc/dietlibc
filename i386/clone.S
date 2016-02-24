#include "syscalls.h"

/* this code is mostly taken from musl, because their implementation was
slightly shorter than mine */

.text
.type clone,@function
.weak clone
clone:
.type __clone,@function
.global __clone
__clone:
  push %ebp	/* create stack frame */
  mov %esp,%ebp

  push %ebx	/* save callee-saves registers from abi */
  push %esi
  push %edi

  /* This is a struct user_desc from asm/ldt.h;
     Since we are pushing it on the stack, and the stack grows downward,
     we push the last element first. Honestly, I don't understand why we
     use the value of gs>>3, but that's what glibc does. */
  xor %eax,%eax
  push $0x51
  mov %gs,%ax
  push $0xfffff
  shr $3,%eax
  push 28(%ebp)
  push %eax

  /* now prepare the syscall */
  /* difficulty: arguments differ.
     C clone: [ebp+8] func, [ebp+12] stack, [ebp+16] flags, [ebp+20] arg, [ebp+24] ptid, [ebp+28] tls, [ebp+32] ctid
     syscall: [ebx] flags, [ecx] stack, [edx] ptid, [esi] ctid, [edi] regs */
  mov $__NR_clone,%al	/* eax = syscall number */
  mov 12(%ebp),%ecx	/* ecx = stack */
  mov 16(%ebp),%ebx	/* ebx = flags */

  and $-16,%ecx		/* make sure the stack is 16-aligned */
  sub $16,%ecx

  mov 20(%ebp),%edi	/* [stack] = arg */
  mov %edi,(%ecx)
  mov 24(%ebp),%edx	/* edx = ptid */
  mov %esp,%esi		/* esi = [the struct we put on the stack above] */
  mov 32(%ebp),%edi	/* edi = ctid */
  mov 8(%ebp),%ebp	/* ebp = the thread function to call */
  int $0x80		/* do the syscall */
  test %eax,%eax
  jnz .Lparent_or_error

  /* the thread creation worked, and we are in the new thread */
  /* our stack is the new stack we passed in edi above, and we wrote the arg on the stack */
  /* we saved the function to call into ebp, so we can call it now */
  mov %ebp,%eax		/* calling with ebp=0 lets gdb know this is the end of the stack */
  xor %ebp,%ebp
  call *%eax
  /* If we get here, the thread function returned.
     This should never happen with thrd_create */
  mov %eax,%ebx		/* put function return code into 1st arg to exit() */
  xor %eax,%eax
  inc %eax		/* __NR_exit == 1 */
  int $0x80	/* call exit, does not return */

.Lparent_or_error:
  /* if we get here, clone either returned a positive number (then we
  created a new thread and the number is its TID), or a negative number
  (which we need to convert to -1+errno). First, clean up the stack. */
  lea 16(%esp),%esp
  pop %edi
  pop %esi
  pop %ebx
  pop %ebp
  ja .Lparent
  jmp __error_unified_syscall
.Lparent:
  ret
.size __clone, .-__clone
