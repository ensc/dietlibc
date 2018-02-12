#include <sys/cdefs.h>
// #define extern __hidden__

// #define PIEDEBUG

extern char __executable_start;

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
#include <string.h>
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
extern __hidden__ unsigned long __guard;
#endif

#if defined(WANT_VALGRIND_SUPPORT)
#ifdef NDEBUG
#undef WANT_VALGRIND_SUPPORT
#endif
int __valgrind=0;
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



#ifdef __PIE__

#if (__WORDSIZE == 64)

#define phdr Elf64_Phdr
#define ehdr Elf64_Ehdr
#define shdr Elf64_Shdr
#define sym Elf64_Sym
#define dyn Elf64_Dyn
#define rela Elf64_Rela
#define R_SYM ELF64_R_SYM
#define R_TYPE ELF64_R_TYPE

#else

#define phdr Elf32_Phdr
#define ehdr Elf32_Ehdr
#define shdr Elf32_Shdr
#define sym Elf32_Sym
#define dyn Elf32_Dyn
#define rela Elf32_Rela
#define R_SYM ELF32_R_SYM
#define R_TYPE ELF32_R_TYPE

#endif

#ifdef PIEDEBUG
static size_t Strlen(const char* s) {
  size_t i;
  for (i=0; s[i]; ++i) ;
  return i;
}

static ssize_t Write(int fd,const char* s,size_t len) {
  ssize_t r;
  asm("syscall\n" : "=a"(r) : "a" (1), "D" (fd), "S" (s), "d" (len) );
  return r;
}

static void _puts(const char* s) {
  Write(1,s,Strlen(s));
}

static void _putn(size_t n) {
  char buf[100];
  size_t j=sizeof(buf);
  if (!n) buf[--j]='0';
  for (; j>0 && n; ) {
    buf[--j]=(n%10)+'0';
    n/=10;
  }
  Write(1,buf+j,sizeof(buf)-j);
}

static void _putx(size_t n) {
  char buf[100];
  size_t j=sizeof(buf);
  if (!n) buf[--j]='0';
  for (; j>0 && n; ) {
    char x = n%16;
    buf[--j]=x<10 ? x+'0' : x+'a'-10;
    n/=16;
  }
  Write(1,buf+j,sizeof(buf)-j);
}

static void _putphflags(size_t n) {
  char buf[100];
  size_t i=0;
  if (n&4) buf[i++]='R';
  if (n&2) buf[i++]='W';
  if (n&1) buf[i++]='X';
  Write(1,buf,i);
}

struct lookup { size_t n; const char s[20]; };

static void _putl(size_t n,struct lookup* l) {
  size_t i;
  for (i=0; l[i].n!=0; ++i)
    if (l[i].n==n) {
      _puts(l[i].s);
      return;
    }
  Write(1,"[",1);
  _putx(n);
  Write(1,"]",1);
}

static void dump_maps() {
  int fd=open("/proc/self/maps",O_RDONLY);
  char buf[2048];
  if (fd!=-1) {
    int r=read(fd,buf,sizeof(buf));
    if (r>0) write(1,buf,r);
    close(fd);
  }
}

static struct lookup atype[] = {
  { 1, "AT_IGNORE" },
  { 2, "AT_EXECFD" },
  { 3, "AT_PHDR" },
  { 4, "AT_PHENT" },
  { 5, "AT_PHNUM" },
  { 6, "AT_PAGESZ" },
  { 7, "AT_BASE" },
  { 8, "AT_FLAGS" },
  { 9, "AT_ENTRY" },
  { 10, "AT_NOTELF" },
  { 11, "AT_UID" },
  { 12, "AT_EUID" },
  { 13, "AT_GID" },
  { 14, "AT_EGID" },
  { 17, "AT_CLKTCK" },
  { 15, "AT_PLATFORM" },
  { 16, "AT_HWCAP" },
  { 18, "AT_FPUCW" },
  { 19, "AT_DCACHEBSIZE" },
  { 20, "AT_ICACHEBSIZE" },
  { 21, "AT_UCACHEBSIZE" },
  { 22, "AT_IGNOREPPC" },
  { 23, "AT_SECURE" },
  { 24, "AT_BASE_PLATFORM" },
  { 25, "AT_RANDOM" },
  { 26, "AT_HWCAP2" },
  { 31, "AT_EXECFN" },
  { 32, "AT_SYSINFO" },
  { 33, "AT_SYSINFO_EHDR" },
  { 0 },
};

static struct lookup ptype[] = {
  { 1, "PT_LOAD" },
  { 2, "PT_DYNAMIC" },
  { 3, "PT_INTERP" },
  { 4, "PT_NOTE" },
  { 5, "PT_SHLIB" },
  { 6, "PT_PHDR" },
  { 7, "PT_TLS" },
  { 8, "PT_NUM" },
  { 0x60000000, "PT_LOOS" },
  { 0x6474e550, "PT_GNU_EH_FRAME" },
  { 0x6474e551, "PT_GNU_STACK" },
  { 0x6474e552, "PT_GNU_RELRO" },
  { 0x6ffffffa, "PT_LOSUNW" },
  { 0x6ffffffa, "PT_SUNWBSS" },
  { 0x6ffffffb, "PT_SUNWSTACK" },
  { 0x6fffffff, "PT_HISUNW" },
  { 0x6fffffff, "PT_HIOS" },
  { 0x70000000, "PT_LOPROC" },
  { 0x7fffffff, "PT_HIPROC" },
  { 0 } };

static struct lookup dtag[] = {
  { 1, "DT_NEEDED" },
  { 2, "DT_PLTRELSZ" },
  { 3, "DT_PLTGOT" },
  { 4, "DT_HASH" },
  { 5, "DT_STRTAB" },
  { 6, "DT_SYMTAB" },
  { 7, "DT_RELA" },
  { 8, "DT_RELASZ" },
  { 9, "DT_RELAENT" },
  { 10, "DT_STRSZ" },
  { 11, "DT_SYMENT" },
  { 12, "DT_INIT" },
  { 13, "DT_FINI" },
  { 14, "DT_SONAME" },
  { 15, "DT_RPATH" },
  { 16, "DT_SYMBOLIC" },
  { 17, "DT_REL" },
  { 18, "DT_RELSZ" },
  { 19, "DT_RELENT" },
  { 20, "DT_PLTREL" },
  { 21, "DT_DEBUG" },
  { 22, "DT_TEXTREL" },
  { 23, "DT_JMPREL" },
  { 24, "DT_BIND_NOW" },
  { 25, "DT_INIT_ARRAY" },
  { 26, "DT_FINI_ARRAY" },
  { 27, "DT_INIT_ARRAYSZ" },
  { 28, "DT_FINI_ARRAYSZ" },
  { 29, "DT_RUNPATH" },
  { 30, "DT_FLAGS" },
  { 32, "DT_ENCODING" },
  { 32, "DT_PREINIT_ARRAY" },
  { 33, "DT_PREINIT_ARRAYSZ" },
  { 34, "DT_NUM" },
  { 0x6000000d, "DT_LOOS" },
  { 0x6ffff000, "DT_HIOS" },
  { 0x6ffffef5, "DT_GNU_HASH" },
  { 0x6ffffff9, "DT_RELACOUNT" },
  { 0x6ffffffa, "DT_RELCOUNT" },
  { 0x6ffffffb, "DT_FLAGS_1" },
  { 0x70000000, "DT_LOPROC" },
  { 0x7fffffff, "DT_HIPROC" },
  { 0 }
};
#endif

static void callback() {
#ifdef PIEDEBUG
  _puts("callback called!\n");
#endif
  _exit(111);
}

#endif	/* __PIE__ */

// extern size_t _GLOBAL_OFFSET_TABLE_;






#ifdef __PIE__
__hidden__ char _DYNAMIC;
#endif

int stackgap(int argc,char* argv[],char* envp[]);
int stackgap(int argc,char* argv[],char* envp[]) {
#if defined(WANT_STACKGAP) || defined(WANT_SSP) || defined(WANT_TLS)
  long* auxvec=(long*)envp;
  char const * rand=(char*)&auxvec;
  char* tlsdata;
#endif
#ifndef WANT_ELFINFO
  while (*auxvec) ++auxvec;			/* skip envp to get to auxvec */
  ++auxvec;

  _auxvec=auxvec;
#endif
#ifdef WANT_STACKGAP
  unsigned short s;
  volatile char* gap;
#endif
#ifdef __PIE__
  /* This code is meant for static PIE executables. */
  /* Since we do not have an interpreter, we'll have to set up the GOT ourselves. */
  {
    size_t i;
    const char* base=0;
    rela* r=0;
    size_t relasz=0,relaent=0;
    size_t phent=0, phnum=0;
    size_t saddr=0;
    const char* elfimage=0;
    for (i=0; auxvec[i]; i+=2) {
      switch (auxvec[i]) {
      case AT_PHDR: elfimage=(const char*)auxvec[i+1]; break;
      case AT_PHENT: phent=auxvec[i+1]; break;
      case AT_PHNUM: phnum=auxvec[i+1]; break;
      case AT_ENTRY: saddr=auxvec[i+1]; break;
      }
    }
    /* All four must be there, or we are hosed. Not checking. */

#ifdef PIEDEBUG
    dump_maps();
#endif

#ifdef PIEDEBUG
    _puts("auxvec:\n");
    for (i=0; auxvec[i]; i+=2) {
      _putl(auxvec[i],atype);
      _puts(" - ");
      _putx(auxvec[i+1]);
      _puts("\n");
    }
    _puts("\nme=");
    _putx((size_t)&stackgap);
    _puts("\n");
#endif

    if (elfimage) {

      size_t* pltgot=0;
      size_t pltrelsz=0,pltrel=0;

#ifdef PIEDEBUG
      _puts("\nphdr:\n");
      for (i=0; i<phnum; ++i) {
	const phdr* ph=(const phdr*)(elfimage + i*phent);
	_puts("type ");
	_putl(ph->p_type,ptype); _puts("\tofs ");
	_putx(ph->p_offset); _puts("\tvaddr ");
	_putx(ph->p_vaddr); _puts("\tpaddr ");
	_putx(ph->p_paddr); _puts("\tfilesz ");
	_putx(ph->p_filesz); _puts("\tmemsz ");
	_putx(ph->p_memsz); _puts("\tflags ");
	_putphflags(ph->p_flags); _puts("\talign ");
	_putx(ph->p_align); _puts("\n");
      }
      _puts("\n\n");
#endif

      for (i=0; i<phnum; ++i) {
	const phdr* ph=(const phdr*)(elfimage + i*phent);
	if (ph->p_type==PT_DYNAMIC) {	/* found .dynamic */
	  const dyn* dh;
	  size_t j;
	  
	  base = &_DYNAMIC - ph->p_vaddr;
#ifdef PIEDEBUG
	  if (((ehdr*)base)->e_entry + (uintptr_t)base != saddr) {
	    _puts("fail: ");
	    _putx(((ehdr*)base)->e_entry + (uintptr_t)base);
	    _puts(" != ");
	    _putx(saddr);
	    _puts("!\n\n");
	  }
#endif

	  for (j=0; j<ph->p_memsz; j+=sizeof(dyn)) {
	    dh=(const dyn*)(base + ph->p_vaddr + j);
	    if (!dh->d_tag) break;
#ifdef PIEDEBUG
	    _putl(dh->d_tag,dtag); _puts(" - "); _putx(dh->d_un.d_val); _puts("\n");
#endif
	    switch (dh->d_tag) {
	    case DT_PLTGOT: pltgot=(size_t*)(base+dh->d_un.d_ptr); break;
	    case DT_PLTRELSZ: pltrelsz=dh->d_un.d_val; break;
	    case DT_PLTREL: pltrel=dh->d_un.d_val; break;

	    case DT_RELA: r=(rela*)(base+dh->d_un.d_ptr); break;
	    case DT_RELASZ: relasz=dh->d_un.d_val; break;
	    case DT_RELAENT: relaent=dh->d_un.d_val; break;
	    }
	  }
	  break;
	}
      }

#ifdef PIEDEBUG
      _puts("got pltgot "); _putx((uintptr_t)pltgot); _puts(", pltrelsz "); _putn(pltrelsz); _puts(", pltrel "); _putn(pltrel); _puts("\n");
#endif
      if (r && relasz && relaent) {
#ifdef PIEDEBUG
	_puts("got rela ");
	_putx((uintptr_t)r);
	_puts(", relasz ");
	_putx(relasz);
	_puts(", relaent ");
	_putx(relaent);
	_puts("\n");

	_puts("\nrela dump:\n");
#endif

	for (i=relasz/relaent; i; --i) {

#ifdef PIEDEBUG
	  _puts("@ "); _putx((uintptr_t)base + r->r_offset);
	  _puts(", ofs "); _putx(r->r_offset);
	  _puts(", info "); _putx(r->r_info);
	  _puts(", addend "); _putx(r->r_addend);
	  _puts("\n");
#endif

#if __WORDSIZE == 64
	  unsigned int type = ELF64_R_TYPE(r->r_info);
#else
	  unsigned int type = ELF32_R_TYPE(r->r_info);
#endif
	  size_t* x=(size_t*)((char*)(base+r->r_offset));
	  switch (type) {
#if defined(__x86_64__)
	  case R_X86_64_RELATIVE: *x = (uintptr_t)base + r->r_addend; break;
#else
#error architecture not supported yet (edit stackgap.c)
#endif
#ifdef PIEDEBUG
	  default: _puts("unsupported relocation type "); _putx(type); break;
#endif
	  }
	  ++r;
	}
	if (pltgot)
	  pltgot[2]=(uintptr_t)callback;
      }
    }
  }
#endif

#if defined(WANT_STACKGAP) || defined(WANT_SSP) || defined(WANT_TLS)
#if defined(WANT_STACKGAP) || defined(WANT_SSP)
  rand=find_in_auxvec(auxvec,25);
#ifdef WANT_URANDOM_SSP
  if (!rand) {
    void *tmp=alloca(10);
    rand=tmp;
    int fd=open("/dev/urandom",O_RDONLY);	// If this fails, there is not much we can do. Limp on.
    read(fd,tmp,10);
    close(fd);
  }
#endif
#endif
#ifdef WANT_STACKGAP
#ifdef __UNALIGNED_MEMORY_ACCESS_OK
  s=*(unsigned short*)(rand+8);
#else
  s=rand[8]+(rand[9]<<8);
#endif	// __UNALIGNED_MEMORY_ACCESS_OK
#endif	// WANT_STACKGAP
#ifdef WANT_SSP
#ifdef __UNALIGNED_MEMORY_ACCESS_OK
  __guard=*(unsigned long*)rand;
#else
  memcpy(&__guard,rand,sizeof(__guard));
#endif	// __UNALIGNED_MEMORY_ACCESS_OK
#endif	// WANT_SSP
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
  /* detect valgrind by looking for "valgrind" in $LD_PRELOAD */
  /* for i386 and x86_64 we do it inline instead of calling getenv to
   * reduce bloat */
#if defined(__i386__) || defined(__x86_64__)
  {
    char** e;
    for (e=environ; *e; ++e) {
      if (*(uint64_t*)*e == *(uint64_t*)"LD_PRELO") {
	char* x;
	for (x=*e; *x; ++x) {
	  if (*(uint64_t*)x == *(uint64_t*)"valgrind") {
	    __valgrind=1;
	    goto found;
	  }
	}
      }
    }
found: ;
  }
#else
  {
    const char* v=getenv("LD_PRELOAD");
    __valgrind=(v && strstr(v,"valgrind"));
  }
#endif
#endif
#ifdef WANT_GNU_STARTUP_BLOAT
  program_invocation_name=argv[0];
  {
    char* c;
    for (c=program_invocation_short_name=program_invocation_name; *c; ++c)
      if (*c=='/') program_invocation_short_name=c+1;
  }
#endif
  exit(main(argc,argv,envp));
}
