#include "syscalls.h"

.weak __libc_accept
syscall(accept,accept)
