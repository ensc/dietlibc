#include "syscalls.h"

syscall_weak(sigprocmask,__old_sigprocmask,__n_sigprocmask)
