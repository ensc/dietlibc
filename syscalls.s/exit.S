#include "syscalls.h"

#ifndef __i386__
#ifndef __hppa__
syscall_weak(exit,exit,_exit)
#endif
#endif
