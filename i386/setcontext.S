#include "syscalls.h"

.text
.global __setcontext_trampoline
.type __setcontext_trampoline,@function
__setcontext_trampoline:
  mov %ebx,4(%esp)
  /* fall through */
.size __setcontext_trampoline,.-__setcontext_trampoline

.global setcontext
.type setcontext,@function
/* switch to stored CPU context from struct ucontext* */
setcontext:
  movl 4(%esp),%eax	/* load ucontext* */
  /* first, load the signal mask */
  movl %ebx,%edi
  xorl %edx,%edx
  movl $2,%ebx	/* SIG_SETMASK */
  lea 108(%eax),%ecx
  movl $__NR_sigprocmask,%eax
  int $0x80
  cmp $-4095,%eax
  jb .Lnoerror
  jmp __error_unified_syscall
.Lnoerror:
  movl 4(%esp),%eax	/* load ucontext* */
  mov %edi,%ebx
  lea 236(%eax),%ecx
  fldenv (%ecx)

  /* the registers are in uc_mcontext, which starts at offset 20 */
  /* do not restore gs because it's thread-specific */
  movw 24(%eax),%fs
  movw 28(%eax),%es
  movw 32(%eax),%ds
  movl 36(%eax),%edi
  movl 40(%eax),%esi
  movl 44(%eax),%ebp
  movl 48(%eax),%esp
  movw 92(%eax),%ss
  movl 52(%eax),%ebx
  movl 56(%eax),%edx
  movl 60(%eax),%ecx

  movl 76(%eax),%eax
  jmp *%eax
.size setcontext, .-setcontext
