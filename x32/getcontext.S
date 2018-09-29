#include "syscalls.h"

.text
.global getcontext
#ifdef __PIE__
.hidden getcontext
#endif
.type getcontext,@function
/* store current CPU context into struct ucontext* */
getcontext:
  /* the registers are in uc_mcontext, which starts at offset 20 */
  movl %r8d,20(%edi)
  movl %r9d,24(%edi)
//  movl %r10d,28(%edi)
//  movl %r11d,32(%edi)
  movl %r12d,36(%edi)
  movl %r13d,40(%edi)
  movl %r14d,44(%edi)
  movl %r15d,48(%edi)
  movl %edi,52(%edi)	/* what a useless statement :-) */
  movl %esi,56(%edi)
  movl %ebp,60(%edi)
  movl %ebx,64(%edi)
  movl %edx,68(%edi)
  movl $0,72(%edi)	/* store 0 as value of rax because that's the return value of the function */
  movl %ecx,76(%edi)
  leal 8(%esp),%eax
  movl %eax,80(%edi)
  movl (%rsp),%eax
  movl %eax,84(%edi)	/* store function return address as rip */
  lahf
  movl %eax,88(%edi)
  /* valgrind dies if we use these instructions */
  movw %cs,92(%edi)
  movw %gs,94(%edi)
  movw %fs,96(%edi)
  /* write nothing to err (100) and trapno (104) */

  leal 280(%edi),%eax	// __fpregs_mem
  movl %eax,116(%edi)	// uc_mcontext.fpstate
  fnstenv (%eax)
  fldenv (%eax)
  stmxcsr 304(%rdi)	// __fpregs_mem.mxcsr

  /* now call sigprocmask(SIG_BLOCK, NULL, &uc->uc_sigmask) */
  leal 152(%edi),%edx
  xorl %esi,%esi
  xorl %edi,%edi
  jmp sigprocmask
.size getcontext, .-getcontext
