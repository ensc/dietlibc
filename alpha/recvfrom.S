#include "syscalls.h"

.weak __libc_recvfrom
syscall(recvfrom,recvfrom)
