#include "syscalls.h"

.text
.global __munmap_and_exit
#ifdef __PIE__
.hidden __munmap_and_exit
#endif
.type __munmap_and_exit,@function
__munmap_and_exit:
  mov $__NR_munmap,%eax
  syscall
  mov $__NR_exit,%eax
  xor %edi,%edi
  syscall
.Lhere:
.size __munmap_and_exit, .Lhere-__munmap_and_exit
