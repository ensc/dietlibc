#include "syscalls.h"

.weak __libc_fcntl
__libc_fcntl:
syscall(fcntl,fcntl)
