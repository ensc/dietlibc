#ifndef _SYS_MMAN_H
#define _SYS_MMAN_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <linux/mman.h>
#include <asm/page.h>

#ifndef PAGE_SIZE
#warning PAGE_SIZE undefined
#endif

#define MAP_FAILED      ((void *) -1)

extern void *mmap (void *__addr, size_t __len, int __prot,
                   int __flags, int __fd, off_t __offset);

extern int munmap (void *__addr, size_t __len) __THROW;
extern int mprotect (void *__addr, size_t __len, int __prot) __THROW;
extern int msync (void *__addr, size_t __len, int __flags) __THROW;
extern void *mremap (void *__addr, size_t __old_len, size_t __new_len,
		     int __may_move) __THROW;
extern int mincore (void *__start, size_t __len, unsigned char *__vec);

#ifdef __quad_t
extern void *mmap64 (void *__addr, size_t __len, int __prot,
		     int __flags, int __fd, off64_t __offset) __THROW;
#endif

int mlockall(int flags) __THROW;
int munlockall(void) __THROW;

#endif
