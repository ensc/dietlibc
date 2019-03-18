#include "syscalls.h"

.text

.global __setcontext_trampoline
#ifdef __PIE__
.hidden __setcontext_trampoline
.hidden setcontext
#endif
.type __setcontext_trampoline,@function
__setcontext_trampoline:
  movq %rbx,%rdi
  /* fall through */
  .size __setcontext_trampoline, .-__setcontext_trampoline

.global setcontext
.type setcontext,@function
/* load current CPU context from struct ucontext* */
setcontext:
  /* first restore the signal mask, call sigprocmask(SIG_SETMASK,&uc->uc_sigmask,NULL) */
  movl %edi,%ebx
  leal 152(%edi),%esi
  xorl %edx,%edx
  movl $2,%edi
  call sigprocmask
  movl %ebx,%edi
  test %eax,%eax
  jc .Lexit

  /* now the floating point context */
  movq 116(%rdi),%rax	// __fpregs_mem
  fldenv (%rax)
  ldmxcsr 304(%rdi)	// __fpgregs_mem.mxcsr

  /* the registers are in uc_mcontext, which starts at offset 40 */
  movl 20(%edi),%r8d
  movl 24(%edi),%r9d
  movl 36(%edi),%r12d
  movl 40(%edi),%r13d
  movl 44(%edi),%r14d
  movl 48(%edi),%r15d

  movl 56(%edi),%esi
  movl 60(%edi),%ebp
  movl 64(%edi),%ebx
  movl 68(%edi),%edx
  movl 76(%edi),%ecx
  movl 80(%edi),%esp
  push 84(%edi)	/* push return value so ret goes to the right place */

//  movw 186(%rdi),%gs
//  movw 188(%rdi),%fs

  movq 52(%rdi),%rdi

  xorl %eax,%eax

.Lexit:
  ret
.size setcontext, .-setcontext
