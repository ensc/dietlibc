#include <linux/types.h>

int mmap(void*start,size_t length,int prot,int flags,int fd,off_t offset) {
  __asm__ volatile (
    "stmdb   sp!, {r0, r1, r2, r3}\n"
    "mov     r0, sp\n"
    "swi     0x0090005a\n"
    "b	__unified_syscall\n");
}

