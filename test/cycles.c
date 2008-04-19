#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/wait.h>

#if defined (__i386__)
#define RDTSC(dst) asm volatile ("rdtsc" : "=A" (dst))
#elif defined (__x86_64__)
#define RDTSC(dst) do {                                                        \
  uint32_t l, h;                                                               \
  asm volatile ("rdtsc" : "=a" (l), "=d" (h)); \
  dst = (((uint64_t)h) << 32) | l;                             \
} while (0)
#elif defined (__powerpc64__)
#define RDTSC(dst) asm volatile ("mftb %0" : "=r" (dst))
#elif defined (__powerpc__)
#define RDTSC(dst) do { \
  uint32_t chk, tbl, tbu; \
  /* The code below is as suggested in Motorola reference manual for 32 bits PPCs. */ \
  __asm__ __volatile__ ("1: mftbu %0; mftb %1; mftbu %2; cmpw %2,%0; bne 1b" \
    : "=r" (tbu), "=r" (tbl), "=r" (chk) ); \
  dst = ((uint64_t)tbu << 32) | tbl; \
} while (0)
#elif defined (__sparcv9__)
#define RDTSC(dst) asm volatile ("rd %%tick, %%0":"=r"(dst))
#else
#warning "Unimplemented rdtsc"
#define RDTSC(dst) dst = 0
#endif

extern char **environ;

int main(int argc,char* argv[]) {
  uint64_t a,b;
  RDTSC(a);
  if (!fork()) { execve(argv[1],argv+1,environ); exit(1); }
  wait(0);
  RDTSC(b);
  printf("%llu cycles\n",(unsigned long long)(b-a));

  return 0;
}
