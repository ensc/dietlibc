#include "syscalls.h"

#ifdef __NR__llseek
.global llseek
llseek:
syscall(_llseek,_llseek)
#endif
