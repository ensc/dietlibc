#include "syscalls.h"

.text
.global getcontext
#ifdef __PIE__
.hidden getcontext
#endif
.type getcontext,@function
/* store current CPU context into struct ucontext* */
getcontext:
  /* the registers are in uc_mcontext, which starts at offset 40 */
  movq %r8,40(%rdi)
  movq %r9,48(%rdi)
//  movq %r10,56(%rdi)
//  movq %r11,64(%rdi)
  movq %r12,72(%rdi)
  movq %r13,80(%rdi)
  movq %r14,88(%rdi)
  movq %r15,96(%rdi)
  movq %rdi,104(%rdi)	/* what a useless statement :-) */
  movq %rsi,112(%rdi)
  movq %rbp,120(%rdi)
  movq %rbx,128(%rdi)
  movq %rdx,136(%rdi)
  movq $0,144(%rdi)	/* store 0 as value of rax because that's the return value of the function */
  movq %rcx,152(%rdi)
  leaq 8(%rsp),%rax
  movq %rax,160(%rdi)
  movq (%rsp),%rax
  movq %rax,168(%rdi)	/* store function return address as rip */
  lahf
  movq %rax,176(%rdi)
  /* valgrind dies if we use these instructions */
  movw %cs,184(%rdi)
  movw %gs,186(%rdi)
  movw %fs,188(%rdi)
  /* write nothing to err (192) and trapno (200) */

  leaq 424(%rdi),%rax
  movq %rax,224(%rdi)
  fnstenv (%rax)
  fldenv (%rax)
  stmxcsr 448(%rdi)

  /* now call sigprocmask(SIG_BLOCK, NULL, &uc->uc_sigmask) */
  leaq 296(%rdi),%rdx
  xorl %esi,%esi
  xorl %edi,%edi
  jmp sigprocmask
.size getcontext, .-getcontext
