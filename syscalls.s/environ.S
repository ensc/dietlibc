.section ".bss"
.align 8
#include <endian.h>
#include <dietfeatures.h>

.type environ,object
.weak environ
.type __environ,object
.weak __environ
__environ:
environ:
#if __WORDSIZE == 64
	.quad 0
#else
	.long 0
#endif
.size environ,.-environ
.size __environ,.-__environ

/* __elfinfo will be initialized in start.S to point to the
   terminating NULL of the environment. */

#ifdef WANT_ELFINFO
.type __elfinfo,object
.weak __elfinfo
__elfinfo:
#if __WORDSIZE == 64
	.quad 0
#else
	.long 0
#endif
.size __elfinfo,.-__elfinfo
#endif
