#ifndef _ASM_SIGCONTEXT_H
#define _ASM_SIGCONTEXT_H

#ifdef __i386__
#include <asm/i386-sigcontext.h>
#endif

#ifdef __sparc__
#include <asm/sparc-sigcontext.h>
#endif

#ifdef __mips__
#include <asm/mips-sigcontext.h>
#endif

#ifdef __powerpc__
#include <asm/ppc-sigcontext.h>
#endif

#ifdef __alpha__
#include <asm/alpha-sigcontext.h>
#endif

#ifdef __arm__
#include <asm/arm-sigcontext.h>
#endif

#ifdef __hppa__
#include <asm/parisc-sigcontext.h>
#endif

#endif
