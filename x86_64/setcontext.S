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
  movq %rdi,%rbx
  leaq 296(%rdi),%rsi
  xorl %edx,%edx
  movl $2,%edi
  call sigprocmask
  mov %rbx,%rdi
  test %rax,%rax
  jc .Lexit

  /* now the floating point context */
  movq 224(%rdi),%rax
  fldenv (%rax)
  ldmxcsr 448(%rdi)

  /* the registers are in uc_mcontext, which starts at offset 40 */
  movq 40(%rdi),%r8
  movq 48(%rdi),%r9
  movq 72(%rdi),%r12
  movq 80(%rdi),%r13
  movq 88(%rdi),%r14
  movq 96(%rdi),%r15

  movq 112(%rdi),%rsi
  movq 120(%rdi),%rbp
  movq 128(%rdi),%rbx
  movq 136(%rdi),%rdx
  movq 152(%rdi),%rcx
  movq 160(%rdi),%rsp
  pushq 168(%rdi)	/* push return value so ret goes to the right place */

//  movw 186(%rdi),%gs
//  movw 188(%rdi),%fs

  movq 104(%rdi),%rdi

  xorl %eax,%eax

.Lexit:
  ret
.size setcontext, .-setcontext
