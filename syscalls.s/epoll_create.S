#include "syscalls.h"

#ifdef __NR_sys_epoll_create
syscall(sys_epoll_create,epoll_create)
#endif
