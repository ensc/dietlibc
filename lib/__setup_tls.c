#include "dietfeatures.h"
#include <unistd.h>
#include <sys/tls.h>

void __setup_tls(tcbhead_t* mainthread);

#if defined(WANT_SSP)
extern __hidden__ unsigned long __guard;
#endif

void __setup_tls(tcbhead_t* mainthread) {
  mainthread->tcb=mainthread;
  mainthread->dtv=0;
  mainthread->self=0;
  mainthread->multiple_threads=0;
#if defined(WANT_SSP)
  mainthread->stack_guard=__guard;
#endif

#if defined(__x86_64__)

  arch_prctl(ARCH_SET_FS, mainthread);

#elif defined(__i386__)

extern int __modern_linux;

  unsigned int sd[4];
  sd[0]=-1;
  sd[1]=(unsigned long int)mainthread;
  sd[2]=0xfffff; /* 4 GB limit */
  sd[3]=0x51; /* bitfield, see struct user_desc in asm-i386/ldt.h */
  if (__modern_linux>=0) {
    if (set_thread_area((struct user_desc*)(void*)&sd)==0) {
      asm volatile ("movw %w0, %%gs" :: "q" (sd[0]*8+3));
      __modern_linux=1;
    } else
      __modern_linux=-1;
  }

#elif defined(__alpha__) || defined(__s390__)
  __builtin_set_thread_pointer(mainthread);
#elif defined(__mips__)
  set_thread_area((char*)(void *)mainthread);
#elif defined(__aarch64__)
  asm volatile ("msr tpidr_el0, %0" :: "r"(mainthread));
#elif defined(__arm__)
  __arm_set_tls(mainthread);
#elif defined(__hppa__)
  /* control register 27 is used as thread pointer on PA-RISC Linux,
   * but it can only be set from Ring0. The Linux kernel provides
   * privileged code to set this register, so call that. (cf. syscalls,
   * which branch to 0x100(%%sr2, %%r0), instead.) PA-RISC has
   * 1-instruction delayed branching. The register may be read by any
   * code however (using mfctl %cr27, %rXX). r26 is used as input for
   * the kernel code, r31 is the return address pointer set by the
   * branch instruction, so clobber both. */
  asm volatile ("ble 0xe0(%%sr2, %%r0)\n\t"
                "copy %0, %%r26"
                :: "r" (mainthread) : "r26", "r31");
#elif defined(__ABI_TLS_REGISTER)
  register tcbhead_t* __thread_self __asm__(__ABI_TLS_REGISTER);
  __thread_self=mainthread;
  __asm__ __volatile__("" : : "r"(__thread_self) : "memory");
#else
#error "no idea how to enable TLS on this platform, edit lib/stackgap-common.h"
#endif
}
