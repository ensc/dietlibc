#include "syscalls.h"

#ifndef __i386__
syscall_weak(exit,exit,_exit)
#endif
