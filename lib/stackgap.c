/* Warning: this code sets up the machine registers and segments needed
 * for -fstack-protector to work.  If you compile this function with
 * -fstack-protector, it will reference those registers before they are
 * set up properly, causing a segmentation fault.  Ubuntu adds
 * -fstack-protector to their gcc default options, so this breaks.  I
 * added a workaround to the Makefile to make sure this code is always
 * compiled with -fno-stack-protector for this reason.  Or, as a friend
 * put it: yo dawg. I herd u liek stack protektion. :-)
 */
#include <unistd.h>
#include <fcntl.h>
#include <alloca.h>
#include <sys/time.h>
#include <sys/tls.h>
#include <endian.h>
#include <elf.h>
#include <stdlib.h>
#include "dietfeatures.h"
#include <sys/auxv.h>
#include "dietelfinfo.h"

#ifdef IN_LDSO
#undef WANT_TLS
#undef WANT_SSP
#endif

#ifdef WANT_GNU_STARTUP_BLOAT
char* program_invocation_name;
char* program_invocation_short_name;
#endif

void const * __vdso;

extern int main(int argc,char* argv[],char* envp[]);

#if defined(WANT_SSP)
extern unsigned long __guard;
#endif

#if defined(WANT_VALGRIND_SUPPORT)
int __valgrind=1;
#endif

#ifdef __i386__
int __modern_linux;
#endif

#ifdef WANT_TLS
/* __tdatasize is the size of the initialized thread local data section
 * __tmemsize is the size of the complete thread local data section
 *   (including uninitialized data)
 * __tdataptr is a pointer to the initialized thread local data section
 * __tmemsize is already rounded up to meet alignment
 * the final memory layout is [tdata] [tbss (zero)] [tcb] */
size_t __tdatasize, __tmemsize;
void* __tdataptr;

static void findtlsdata(long* auxvec) {
#if (__WORDSIZE == 64)
  Elf64_Phdr const * x=0;
#else
  Elf32_Phdr const * x=0;
#endif
  size_t i,n=0;
#ifndef WANT_ELFINFO
  while (*auxvec) {
    if (auxvec[0]==3) {	/* AT_PHDR */
      x=(void*)auxvec[1];
      if (n) break;
    } else if (auxvec[0]==5) { /* AT_PHNUM */
      n=auxvec[1];
      if (x) break;
    }
    auxvec+=2;
  } /* if we don't find the entry, the kernel let us down */
#else
  {
    __diet_elf_addr_t const	*x_addr = __get_elf_aux_value(AT_PHDR);
    __diet_elf_addr_t const	*n_addr = __get_elf_aux_value(AT_PHNUM);

    (void)auxvec;
    if (x_addr)
      x = (__typeof__(x)) *x_addr;
    if (n_addr)
      n = *n_addr;
  }
#endif
  if (!x || !n) return;	/* a kernel this old does not support thread local storage anyway */
  for (i=0; i<n; ++i)
    if (x[i].p_type==PT_TLS) {
      __tdataptr=(void*)x[i].p_vaddr;
      __tdatasize=x[i].p_filesz;
      __tmemsize=x[i].p_memsz;
      if (__tmemsize&15)
	__tmemsize=(__tmemsize+15)&~15;
      break;
    }
  /* if there is no PT_TLS section, there is no thread-local data, and
   * we just leave the __t* variables zero */
}
#endif

#if defined(WANT_SSP) || defined(WANT_TLS)
tcbhead_t* __tcb_mainthread;

void __setup_tls(tcbhead_t*);

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
#elif defined(__arm__)
  __arm_set_tls(mainthread);
#elif defined(__ABI_TLS_REGISTER)
  register tcbhead_t* __thread_self __asm__(__ABI_TLS_REGISTER);
  __thread_self=mainthread;
  __asm__ __volatile__("" : : "r"(__thread_self) : "memory");
#else
#error "no idea how to enable TLS on this platform, edit lib/stackgap.c"
#endif
}
#endif

static void const * find_in_auxvec(long* x,long what) {
#ifndef WANT_ELFINFO
  while (*x) {
    if (*x==what)
      return (void*)x[1];
    x+=2;
  }
  return NULL;
#else
  __diet_elf_addr_t const	*a = __get_elf_aux_value(what);
  (void)x;
  return a ? (void *)*a : NULL;
#endif
}

#ifndef WANT_ELFINFO
static long* _auxvec;
#else
#define _auxvec	NULL
#endif

unsigned long getauxval(unsigned long type) {
  return (long)find_in_auxvec(_auxvec,type);
}

int stackgap(int argc,char* argv[],char* envp[]);
int stackgap(int argc,char* argv[],char* envp[]) {
#if defined(WANT_STACKGAP) || defined(WANT_SSP) || defined(WANT_TLS)
  char const * rand;
  char* tlsdata;
  long* auxvec=(long*)envp;
#ifndef WANT_ELFINFO
  while (*auxvec) ++auxvec; ++auxvec;	/* skip envp to get to auxvec */
  _auxvec=auxvec;
#endif
#ifdef WANT_STACKGAP
  unsigned short s;
  volatile char* gap;
#endif
#if defined(WANT_STACKGAP) || defined(WANT_SSP)
  char myrand[10];
  rand=find_in_auxvec(auxvec,25);
  if (!rand) {
    int fd=open("/dev/urandom",O_RDONLY);
    read(fd,myrand,10);
    close(fd);
    rand=myrand;
  }
#endif
#ifdef WANT_STACKGAP
  s=*(unsigned short*)(rand+8);
#endif
#ifdef WANT_SSP
  __guard=*(unsigned long*)rand;
#endif
#ifdef WANT_STACKGAP
  gap=alloca(s);
#endif
#endif

  __vdso=find_in_auxvec(auxvec,33);	// AT_SYSINFO_EHDR -> vdso start address
#ifdef __x86_64__
  if (!__vdso) __vdso=(char*)0xffffffffff600000;
#endif

#ifdef WANT_TLS
  findtlsdata(auxvec);
  if (__unlikely(__tmemsize+sizeof(tcbhead_t)<sizeof(tcbhead_t)) ||
      __unlikely(__tmemsize>512*1024*1024) ||
      __unlikely(__tmemsize<__tdatasize))
    return 111;
  tlsdata=alloca(__tmemsize+sizeof(tcbhead_t));
  memcpy(tlsdata,__tdataptr,__tdatasize);
  memset(tlsdata+__tdatasize,0,__tmemsize-__tdatasize);
  __setup_tls(__tcb_mainthread=(tcbhead_t*)(tlsdata+__tmemsize));
  __tcb_mainthread->sysinfo=(uintptr_t)find_in_auxvec(auxvec,32);
#elif defined(WANT_SSP)
  tlsdata=alloca(sizeof(tcbhead_t));
  __setup_tls(__tcb_mainthread=(tcbhead_t*)(tlsdata));
#endif
#if defined(WANT_VALGRIND_SUPPORT)
  {
    const char* v=getenv("LD_PRELOAD");
    __valgrind=(v && strstr(v,"valgrind"));
  }
#endif
#ifdef WANT_GNU_STARTUP_BLOAT
  program_invocation_name=argv[0];
  {
    char* c;
    for (c=program_invocation_short_name=program_invocation_name; *c; ++c)
      if (*c=='/') program_invocation_short_name=c+1;
  }
#endif
  return main(argc,argv,envp);
}

