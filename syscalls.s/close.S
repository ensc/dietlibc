#include "syscalls.h"

.weak __libc_close
__libc_close:
syscall(close,close)
