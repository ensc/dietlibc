#include "syscalls.h"

.weak __libc_write
__libc_write:
syscall(write,write)
