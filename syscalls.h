#ifndef __ASSEMBLER__

#include <linux/unistd.h>

#else

#ifdef __i386__

#include "i386/syscalls.h"

#endif

#ifdef __sparc__

#include "sparc/syscalls.h"

#endif

#ifdef __powerpc__

#include "ppc/syscalls.h"

#endif


#ifdef __mips__

#include "mips/syscalls.h"

#endif

#ifdef __arm__

#include "arm/syscalls.h"

#endif

#ifdef __s390__

#include "s390/syscalls.h"

#endif

#ifdef __alpha__

#include "alpha/syscalls.h"

#endif

#endif
