#include "syscalls.h"

.weak __libc_fork
__libc_fork:
syscall(fork,fork)
