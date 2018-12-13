#include "dietfeatures.h"
#include <endian.h>

.section .ctors,"aw"
.global __CTOR_END__
__CTOR_END__:

.section .dtors,"aw"
.global __DTOR_END__
__DTOR_END__:

.section .eh_frame,"aw"
#if __WORDSIZE == 32
.long 0
#else
.quad 0
#endif
