#ifdef __i386__

#include "i386/syscalls.h"

#endif

#ifdef __sparc__

#ifdef __arch64__
#include "sparc64/syscalls.h"
#else
#include "sparc/syscalls.h"
#endif

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

#ifdef __hppa__

#include "parisc/syscalls.h"

#endif
