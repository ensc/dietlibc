#include "syscalls.h"

.text
.global __munmap_and_exit
#ifdef __PIE__
.hidden __munmap_and_exit
#endif
.type __munmap_and_exit,@function
__munmap_and_exit:
  mov $__NR_munmap,%eax
  mov 4(%esp),%ebx
  mov 8(%esp),%ecx
  int $0x80
  mov $__NR_exit,%eax
  xor %ebx,%ebx
  int $0x80
.Lhere:
.size __munmap_and_exit, .Lhere-__munmap_and_exit
