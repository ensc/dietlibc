#include "syscalls.h"

.weak __libc_pread
syscall(pread,__pread)
