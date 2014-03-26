#ifndef _ASM_PRCTL_H
#define _ASM_PRCTL_H

#if defined(__x86_64__)

#define ARCH_SET_GS 0x1001
#define ARCH_SET_FS 0x1002
#define ARCH_GET_FS 0x1003
#define ARCH_GET_GS 0x1004

#elif defined(__i386__)

#else

#warning "need asm/prctl.h for this platform"
#endif

#endif
