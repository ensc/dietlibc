#ifndef _SETJMP_H
#define _SETJMP_H

#include <sys/cdefs.h>

#ifdef __i386__
#ifndef __ASSEMBLER__
typedef int __jmp_buf[6];
#endif
# define JB_BX	0
# define JB_SI	1
# define JB_DI	2
# define JB_BP	3
# define JB_SP	4
# define JB_PC	5
# define JB_SIZE 24
#endif

#ifdef __alpha__
#define JB_S0  0
#define JB_S1  1
#define JB_S2  2
#define JB_S3  3
#define JB_S4  4
#define JB_S5  5
#define JB_PC  6
#define JB_FP  7
#define JB_SP  8
#define JB_F2  9
#define JB_F3  10
#define JB_F4  11
#define JB_F5  12
#define JB_F6  13
#define JB_F7  14
#define JB_F8  15
#define JB_F9  16
#ifndef __ASSEMBLER__
typedef long int __jmp_buf[17];
#endif
#endif

#ifdef __mips__
#ifndef __ASSEMBLER__
typedef struct
  {
    void * __pc;	/* Program counter.  */
    void * __sp;	/* Stack pointer.  */
    int __regs[8];	/* Callee-saved registers s0 through s7.  */
    void * __fp;	/* The frame pointer.  */
    void * __gp;	/* The global pointer.  */
    int __fpc_csr;	/* Floating point status register.  */
    double __fpregs[6];	/* Callee-saved floating point registers.  */
  } __jmp_buf[1];
#endif
#endif

#ifdef __sparc__
#define JB_SP  0
#define JB_FP  1
#define JB_PC  2
#ifndef __ASSEMBLER__
typedef int __jmp_buf[3];
#endif
#endif

#ifdef __arm__
#define __JMP_BUF_SP            8
#ifndef __ASSEMBLER__
typedef int __jmp_buf[10];
#endif
#endif

#ifdef __powerpc__
# define JB_GPR1   0  /* Also known as the stack pointer */
# define JB_GPR2   1
# define JB_LR     2  /* The address we will return to */
# define JB_GPRS   3  /* GPRs 14 through 31 are saved, 18 in total */
# define JB_CR     21 /* Condition code registers. */
# define JB_FPRS   22 /* FPRs 14 through 31 are saved, 18*2 words total */
# define JB_SIZE   (58*4)
#ifndef __ASSEMBLER__
typedef long int __jmp_buf[58];
#endif
#endif

#ifndef __ASSEMBLER__

//typedef int sig_atomic_t;
#define __sig_atomic_t sig_atomic_t

# define _SIGSET_NWORDS	(1024 / (8 * sizeof (unsigned long int)))
typedef struct
  {
    unsigned long int __val[_SIGSET_NWORDS];
  } __sigset_t;

/* Calling environment, plus possibly a saved signal mask.  */
typedef struct __jmp_buf_tag	/* C++ doesn't like tagless structs.  */
  {
    /* NOTE: The machine-dependent definitions of `__sigsetjmp'
       assume that a `jmp_buf' begins with a `__jmp_buf'.
       Do not move this member or add others before it.  */
    __jmp_buf __jmpbuf;		/* Calling environment.  */
    int __mask_was_saved;	/* Saved the signal mask?  */
    __sigset_t __saved_mask;	/* Saved signal mask.  */
  } jmp_buf[1];

extern int __sigsetjmp (jmp_buf __env, int __savemask) __THROW;

extern void longjmp (jmp_buf __env, int __val)
     __THROW __attribute__ ((__noreturn__));

typedef jmp_buf sigjmp_buf;

extern void siglongjmp (sigjmp_buf __env, int __val)
     __THROW __attribute__ ((__noreturn__));

#define setjmp(env) __sigsetjmp(env,0)
#define sigsetjmp(a,b) __sigsetjmp(a,b)

#endif

#endif
