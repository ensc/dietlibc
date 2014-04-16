#ifndef _SYS_TLS_H
#define _SYS_TLS_H

#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

/* Type for the dtv.  */
typedef union dtv
{
  size_t counter;
  struct {
    void *val;
    bool is_static;
  } pointer;
} dtv_t;

typedef struct
{
  void *tcb;            /* Pointer to the TCB.  Accessed as GS:[0], points to itself. */
  dtv_t *dtv;		/* Data structure used to find thread-local storage */
  void *self;           /* Pointer to the thread descriptor.  */
  int multiple_threads;	/* Set to 0 by _start/stackgap(), set to 1 by thrd_create */
#ifdef __x86_64__
  int gscope_flag;	/* no idea what this is for */
#endif
  uintptr_t sysinfo;		/* not sure why this is here; passed via ELF auxvec, copied here by thrd_create */
  uintptr_t stack_guard;	/* random value used as canary value by gcc -fstack-protector, initialized by _start/stackgap()/thrd_create */
  uintptr_t pointer_guard;	/* random value used by glibc to xor internal pointers as exploit mitigation; not used in dietlibc */
#ifdef __i386__
  int gscope_flag;	/* no idea what this is for */
#endif
} tcbhead_t;

tcbhead_t* __get_cur_tcb(void) __THROW;

#if defined(__i386__)

struct user_desc {
	unsigned int  entry_number;
	unsigned long base_addr;
	unsigned int  limit;
	unsigned int  seg_32bit:1;
	unsigned int  contents:2;
	unsigned int  read_exec_only:1;
	unsigned int  limit_in_pages:1;
	unsigned int  seg_not_present:1;
	unsigned int  useable:1;
};

int set_thread_area(struct user_desc* uinfo);

#elif defined(__x86_64__)

#define ARCH_SET_GS 0x1001
#define ARCH_SET_FS 0x1002
#define ARCH_GET_FS 0x1003
#define ARCH_GET_GS 0x1004

int arch_prctl(unsigned int what, void* where);

#elif defined(__ia64__) || defined(__powerpc64__)

#define __ABI_TLS_REGISTER	"r13"

#elif defined(__powerpc__)

#define __ABI_TLS_REGISTER	"r2"

#elif defined(__sparc__)

#define __ABI_TLS_REGISTER	"%g7"

#elif defined(__arm__)

void __arm_set_tls(void *);

#else

#warning "need proper sys/tls.h for this platform"

#endif

#endif
