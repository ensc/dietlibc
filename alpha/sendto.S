#include "syscalls.h"

.weak __libc_sendto
syscall(sendto,sendto)
