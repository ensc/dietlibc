#include <linuxnet.h>
#include <syscalls.h>
#include "dietfeatures.h"

#if defined(WANT_I386_SOCKETCALL)
__socketcall(accept4,ACCEPT4)
#endif
