#include "syscalls.h"

.weak __libc_sendfile
__libc_sendfile:
syscall(sendfile,sendfile)
