#include "syscalls.h"

.text
.global getcontext
.type getcontext,@function
/* store current CPU context into struct ucontext* */
getcontext:
  movl 4(%esp),%eax	/* load ucontext* */
  /* the registers are in uc_mcontext, which starts at offset 20 */
  movw %gs,20(%eax)
  movw %fs,24(%eax)
  movw %es,28(%eax)
  movw %ds,32(%eax)
  movl %edi,36(%eax)
  movl %esi,40(%eax)
  movl %ebp,44(%eax)
  /* skip esp until we stored ecx */
  movl %ebx,52(%eax)
  movl %edx,56(%eax)
  movl %ecx,60(%eax)
  lea 4(%esp),%ecx	/* store esp+4 to esp */
  mov %ecx,48(%eax)
  movl $0,64(%eax)	/* eax */
  /* not touching trapno and err, no idea what do store there */
  movl (%esp),%ecx
  movl %ecx,76(%eax)	/* store return value from stack into eip of struct */
  movw %cs,80(%eax)
  /* 84: eflags */
  /* 88: esp_at_signal */
  movw %ss,92(%eax)

  lea 236(%eax),%ecx
  movl %ecx,96(%eax)
  fnstenv (%ecx)
  fldenv (%ecx)
  mov %ebx,%edi
  xor %ebx,%ebx
  xor %ecx,%ecx
  lea 108(%eax),%edx
  mov $__NR_sigprocmask,%eax
  int $0x80
  mov %edi,%ebx
  xor %eax,%eax
  ret
.size getcontext, .-getcontext
