#ifndef _SYS_PTRACE_H
#define _SYS_PTRACE_H

#include <sys/cdefs.h>

#define PTRACE_TRACEME		   0
#define PTRACE_PEEKTEXT		   1
#define PTRACE_PEEKDATA		   2
#define PTRACE_PEEKUSR		   3
#define PTRACE_POKETEXT		   4
#define PTRACE_POKEDATA		   5
#define PTRACE_POKEUSR		   6
#define PTRACE_CONT		   7
#define PTRACE_KILL		   8
#define PTRACE_SINGLESTEP	   9

#define PTRACE_ATTACH		0x10
#define PTRACE_DETACH		0x11

#define PTRACE_SYSCALL		  24

#if defined(__i386__)
#define EBX 0
#define ECX 1
#define EDX 2
#define ESI 3
#define EDI 4
#define EBP 5
#define EAX 6
#define DS 7
#define ES 8
#define FS 9
#define GS 10
#define ORIG_EAX 11
#define EIP 12
#define CS  13
#define EFL 14
#define UESP 15
#define SS   16
#define FRAME_SIZE 17

/* this struct defines the way the registers are stored on the 
   stack during a system call. */

struct pt_regs {
  long ebx;
  long ecx;
  long edx;
  long esi;
  long edi;
  long ebp;
  long eax;
  int  xds;
  int  xes;
  long orig_eax;
  long eip;
  int  xcs;
  long eflags;
  long esp;
  int  xss;
};

/* Arbitrarily choose the same ptrace numbers as used by the Sparc code. */
#define PTRACE_GETREGS            12
#define PTRACE_SETREGS            13
#define PTRACE_GETFPREGS          14
#define PTRACE_SETFPREGS          15
#define PTRACE_GETFPXREGS         18
#define PTRACE_SETFPXREGS         19

#define PTRACE_SETOPTIONS         21

/* options set using PTRACE_SETOPTIONS */
#define PTRACE_O_TRACESYSGOOD     0x00000001

#elif defined(__arm__)

#warning I have no idea how I am supposed to support sys/ptrace.h on ARM
#include <asm/ptrace.h>

#elif defined(__alpha__)

struct pt_regs {
  unsigned long r0;
  unsigned long r1;
  unsigned long r2;
  unsigned long r3;
  unsigned long r4;
  unsigned long r5;
  unsigned long r6;
  unsigned long r7;
  unsigned long r8;
  unsigned long r19;
  unsigned long r20;
  unsigned long r21;
  unsigned long r22;
  unsigned long r23;
  unsigned long r24;
  unsigned long r25;
  unsigned long r26;
  unsigned long r27;
  unsigned long r28;
  unsigned long hae;
/* JRP - These are the values provided to a0-a2 by PALcode */
  unsigned long trap_a0;
  unsigned long trap_a1;
  unsigned long trap_a2;
/* These are saved by PAL-code: */
  unsigned long ps;
  unsigned long pc;
  unsigned long gp;
  unsigned long r16;
  unsigned long r17;
  unsigned long r18;
};

struct switch_stack {
  unsigned long r9;
  unsigned long r10;
  unsigned long r11;
  unsigned long r12;
  unsigned long r13;
  unsigned long r14;
  unsigned long r15;
  unsigned long r26;
  unsigned long fp[32];	/* fp[31] is fpcr */
};

#elif defined(__mips__)

/* 0 - 31 are integer registers, 32 - 63 are fp registers.  */
#define FPR_BASE	32
#define PC		64
#define CAUSE		65
#define BADVADDR	66
#define MMHI		67
#define MMLO		68
#define FPC_CSR		69
#define FPC_EIR		70

struct pt_regs {
  /* Pad bytes for argument save space on the stack. */
  unsigned long pad0[6];
  /* Saved main processor registers. */
  unsigned long regs[32];
  /* Other saved registers. */
  unsigned long lo;
  unsigned long hi;
  /* saved cp0 registers */
  unsigned long cp0_epc;
  unsigned long cp0_badvaddr;
  unsigned long cp0_status;
  unsigned long cp0_cause;
};

#elif defined(__sparc__)

struct pt_regs {
  unsigned long psr;
  unsigned long pc;
  unsigned long npc;
  unsigned long y;
  unsigned long u_regs[16]; /* globals and ins */
};

#define UREG_G0        0
#define UREG_G1        1
#define UREG_G2        2
#define UREG_G3        3
#define UREG_G4        4
#define UREG_G5        5
#define UREG_G6        6
#define UREG_G7        7
#define UREG_I0        8
#define UREG_I1        9
#define UREG_I2        10
#define UREG_I3        11
#define UREG_I4        12
#define UREG_I5        13
#define UREG_I6        14
#define UREG_I7        15
#define UREG_WIM       UREG_G0
#define UREG_FADDR     UREG_G0
#define UREG_FP        UREG_I6
#define UREG_RETPC     UREG_I7

/* A register window */
struct reg_window {
  unsigned long locals[8];
  unsigned long ins[8];
};

/* A Sparc stack frame */
struct sparc_stackf {
  unsigned long locals[8];
  unsigned long ins[6];
  struct sparc_stackf *fp;
  unsigned long callers_pc;
  char *structptr;
  unsigned long xargs[6];
  unsigned long xxargs[1];
};

#define TRACEREG_SZ   sizeof(struct pt_regs)
#define STACKFRAME_SZ sizeof(struct sparc_stackf)
#define REGWIN_SZ     sizeof(struct reg_window)

/* These are for pt_regs. */
#define PT_PSR    0x0
#define PT_PC     0x4
#define PT_NPC    0x8
#define PT_Y      0xc
#define PT_G0     0x10
#define PT_WIM    PT_G0
#define PT_G1     0x14
#define PT_G2     0x18
#define PT_G3     0x1c
#define PT_G4     0x20
#define PT_G5     0x24
#define PT_G6     0x28
#define PT_G7     0x2c
#define PT_I0     0x30
#define PT_I1     0x34
#define PT_I2     0x38
#define PT_I3     0x3c
#define PT_I4     0x40
#define PT_I5     0x44
#define PT_I6     0x48
#define PT_FP     PT_I6
#define PT_I7     0x4c

/* Reg_window offsets */
#define RW_L0     0x00
#define RW_L1     0x04
#define RW_L2     0x08
#define RW_L3     0x0c
#define RW_L4     0x10
#define RW_L5     0x14
#define RW_L6     0x18
#define RW_L7     0x1c
#define RW_I0     0x20
#define RW_I1     0x24
#define RW_I2     0x28
#define RW_I3     0x2c
#define RW_I4     0x30
#define RW_I5     0x34
#define RW_I6     0x38
#define RW_I7     0x3c

/* Stack_frame offsets */
#define SF_L0     0x00
#define SF_L1     0x04
#define SF_L2     0x08
#define SF_L3     0x0c
#define SF_L4     0x10
#define SF_L5     0x14
#define SF_L6     0x18
#define SF_L7     0x1c
#define SF_I0     0x20
#define SF_I1     0x24
#define SF_I2     0x28
#define SF_I3     0x2c
#define SF_I4     0x30
#define SF_I5     0x34
#define SF_FP     0x38
#define SF_PC     0x3c
#define SF_RETP   0x40
#define SF_XARG0  0x44
#define SF_XARG1  0x48
#define SF_XARG2  0x4c
#define SF_XARG3  0x50
#define SF_XARG4  0x54
#define SF_XARG5  0x58
#define SF_XXARG  0x5c

/* Stuff for the ptrace system call */
#define PTRACE_SUNATTACH	  10
#define PTRACE_SUNDETACH	  11
#define PTRACE_GETREGS            12
#define PTRACE_SETREGS            13
#define PTRACE_GETFPREGS          14
#define PTRACE_SETFPREGS          15
#define PTRACE_READDATA           16
#define PTRACE_WRITEDATA          17
#define PTRACE_READTEXT           18
#define PTRACE_WRITETEXT          19
#define PTRACE_GETFPAREGS         20
#define PTRACE_SETFPAREGS         21

#define PTRACE_GETUCODE           29  /* stupid bsd-ism */

#elif defined(__powerpc__)

struct pt_regs {
  unsigned long gpr[32];
  unsigned long nip;
  unsigned long msr;
  unsigned long orig_gpr3;	/* Used for restarting system calls */
  unsigned long ctr;
  unsigned long link;
  unsigned long xer;
  unsigned long ccr;
  unsigned long mq;		/* 601 only (not used at present) */
				/* Used on APUS to hold IPL value. */
  unsigned long trap;		/* Reason for being here */
  unsigned long dar;		/* Fault registers */
  unsigned long dsisr;
  unsigned long result;		/* Result of a system call */
};

/*
 * Offsets used by 'ptrace' system call interface.
 * These can't be changed without breaking binary compatibility
 * with MkLinux, etc.
 */
#define PT_R0	0
#define PT_R1	1
#define PT_R2	2
#define PT_R3	3
#define PT_R4	4
#define PT_R5	5
#define PT_R6	6
#define PT_R7	7
#define PT_R8	8
#define PT_R9	9
#define PT_R10	10
#define PT_R11	11
#define PT_R12	12
#define PT_R13	13
#define PT_R14	14
#define PT_R15	15
#define PT_R16	16
#define PT_R17	17
#define PT_R18	18
#define PT_R19	19
#define PT_R20	20
#define PT_R21	21
#define PT_R22	22
#define PT_R23	23
#define PT_R24	24
#define PT_R25	25
#define PT_R26	26
#define PT_R27	27
#define PT_R28	28
#define PT_R29	29
#define PT_R30	30
#define PT_R31	31

#define PT_NIP	32
#define PT_MSR	33
#define PT_CTR	35
#define PT_LNK	36
#define PT_XER	37
#define PT_CCR	38
#define PT_MQ	39

#define PT_FPR0	48	/* each FP reg occupies 2 slots in this space */
#define PT_FPR31 (PT_FPR0 + 2*31)
#define PT_FPSCR (PT_FPR0 + 2*32 + 1)
#endif

extern long int ptrace (int request, ...) __THROW;

#endif
