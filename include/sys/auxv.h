#ifndef SYS_AUXV
#define SYS_AUXV

#include <sys/cdefs.h>

unsigned long getauxval(unsigned long type);

#ifndef AT_BASE
/* don't fail if they already included elf.h */
enum {
  AT_BASE=7,
  AT_BASE_PLATFORM=24,
  AT_CLKTCK=17,
  AT_DCACHEBSIZE=19,
  AT_EGID=14,
  AT_ENTRY=9,
  AT_EUID=12,
  AT_EXECFD=2,
  AT_EXECFN=31,
  AT_FLAGS=8,
  AT_FPUCW=18,
  AT_GID=13,
  AT_HWCAP=16,
  AT_HWCAP2=26,
  AT_ICACHEBSIZE=20,
  AT_PAGESZ=6,
  AT_PHDR=3,
  AT_PHENT=4,
  AT_PHNUM=5,
  AT_PLATFORM=15,
  AT_RANDOM=25,
  AT_SECURE=23,
  AT_SYSINFO=32,
  AT_SYSINFO_EHDR=33,
  AT_UCACHEBSIZE=21,
  AT_UID=11
};
#endif

#endif
