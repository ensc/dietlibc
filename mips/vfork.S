#include "syscalls.h"

/* is this a good idea?  Is this only available if the kernel is
compiled to do emulation? */
syscall_weak(SVR4_vfork,vfork,__libc_vfork)
