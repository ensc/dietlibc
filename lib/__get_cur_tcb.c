#include <sys/tls.h>

tcbhead_t* __get_cur_tcb(void) {
#if defined(__alpha__)
  register void* cur asm("$0");
  asm("call_pal 158" : "=r"(cur) );	/* PAL_rduniq = 158 */
#else	/* alpha */
  register void* cur=0;
#if defined(__sparc__)
  asm("mov %%g6,%0" : "=r"(cur) );	/* %g6 (res. system use) is used as thread pointer */
#elif defined(__s390__)
  asm("ear %0,%%a0" : "=d"(cur) );	/* a0 (access register 0) is used as thread pointer */
#elif defined(__ia64__)
  asm("mov %0 = r13" : "=r"(cur) );	/* r13 (tp) is used as thread pointer */
#elif defined(__x86_64__)
#ifdef __ILP32__
  asm("movl %%fs:(0),%0" : "=r"(cur));
#else
  asm("movq %%fs:(0),%0" : "=r"(cur));
#endif
#elif defined(__i386__)
  asm("movl %%gs:(0),%0" : "=r"(cur));
#endif
#endif /* __alpha__ */
  return cur;
}
