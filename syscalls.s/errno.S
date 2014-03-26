#include "dietfeatures.h"

.section ".bss"
.align 8

#ifdef WANT_TLS
.type errno,tls_object
#else
.type errno,object
#endif
.weak errno
errno:
	.long 0
.size errno,.-errno
