#include "syscalls.h"
#include <linux/types.h>

_syscall3(int,readlink,char*,path,char*,buf,size_t,bufsiz)

