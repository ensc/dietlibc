#include "syscalls.h"

.weak __libc_read
__libc_read:
syscall(read,read)
