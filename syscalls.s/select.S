#include "syscalls.h"

#ifdef __NR__newselect
syscall_weak(_newselect,select,__libc_select)
#elif defined __NR_select
syscall_weak(select,select,__libc_select)
#endif
