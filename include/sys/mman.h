#ifndef _SYS_MMAN_H
#define _SYS_MMAN_H

#include <sys/cdefs.h>
#include <sys/types.h>

#define MREMAP_MAYMOVE	1UL
#define MREMAP_FIXED	2UL

#define PROT_READ	0x1		/* page can be read */
#define PROT_WRITE	0x2		/* page can be written */
#define PROT_EXEC	0x4		/* page can be executed */
#define PROT_NONE	0x0		/* page can not be accessed */

#define MAP_SHARED	0x01		/* Share changes */
#define MAP_PRIVATE	0x02		/* Changes are private */

#ifdef __mips__
#define MAP_FIXED	0x010		/* Interpret addr exactly */
#define MAP_NORESERVE	0x0400		/* don't check for reservations */
#define MAP_ANONYMOUS	0x0800		/* don't use a file */
#define MAP_GROWSDOWN	0x1000		/* stack-like segment */
#define MAP_DENYWRITE	0x2000		/* ETXTBSY */
#define MAP_EXECUTABLE	0x4000		/* mark it as an executable */
#define MAP_LOCKED	0x8000		/* pages are locked */
#define MS_ASYNC	0x0001		/* sync memory asynchronously */
#define MS_INVALIDATE	0x0002		/* invalidate mappings & caches */
#define MS_SYNC		0x0004		/* synchronous memory sync */
#define MCL_CURRENT	1		/* lock all current mappings */
#define MCL_FUTURE	2		/* lock all future mappings */
#define MADV_NORMAL	0x0		/* default page-in behavior */
#define MADV_RANDOM	0x1		/* page-in minimum required */
#define MADV_SEQUENTIAL	0x2		/* read-ahead aggressively */
#define MADV_WILLNEED	0x3		/* pre-fault pages */
#define MADV_DONTNEED	0x4		/* discard these pages */
#else

#ifdef __alpha__
#define MAP_FIXED	0x100		/* Interpret addr exactly */
#define MAP_ANONYMOUS	0x10		/* don't use a file */
#define MAP_GROWSDOWN	0x1000		/* stack-like segment */
#define MAP_DENYWRITE	0x2000		/* ETXTBSY */
#define MAP_EXECUTABLE	0x4000		/* mark it as an executable */
#define MAP_LOCKED	0x8000		/* lock the mapping */
#define MAP_NORESERVE	0x10000		/* don't check for reservations */
#define MS_ASYNC	1		/* sync memory asynchronously */
#define MS_SYNC		2		/* synchronous memory sync */
#define MS_INVALIDATE	4		/* invalidate the caches */
#define MCL_CURRENT	 8192		/* lock all currently mapped pages */
#define MCL_FUTURE	16384		/* lock all additions to address space */
#define MADV_NORMAL	0		/* no further special treatment */
#define MADV_RANDOM	1		/* expect random page references */
#define MADV_SEQUENTIAL	2		/* expect sequential page references */
#define MADV_WILLNEED	3		/* will need these pages */
#define MADV_SPACEAVAIL	5		/* ensure resources are available */
#define MADV_DONTNEED	6		/* dont need these pages */
#else
#define MAP_FIXED	0x10		/* Interpret addr exactly */
#define MAP_ANONYMOUS	0x20		/* don't use a file */
#endif

#if defined(__i386__) || defined(__s390__)
#define MAP_GROWSDOWN	0x0100		/* stack-like segment */
#define MAP_DENYWRITE	0x0800		/* ETXTBSY */
#define MAP_EXECUTABLE	0x1000		/* mark it as an executable */
#define MAP_LOCKED	0x2000		/* pages are locked */
#define MAP_NORESERVE	0x4000		/* don't check for reservations */
#define MS_ASYNC	1		/* sync memory asynchronously */
#define MS_INVALIDATE	2		/* invalidate the caches */
#define MS_SYNC		4		/* synchronous memory sync */
#define MCL_CURRENT	1		/* lock all current mappings */
#define MCL_FUTURE	2		/* lock all future mappings */
#define MADV_NORMAL	0x0		/* default page-in behavior */
#define MADV_RANDOM	0x1		/* page-in minimum required */
#define MADV_SEQUENTIAL	0x2		/* read-ahead aggressively */
#define MADV_WILLNEED	0x3		/* pre-fault pages */
#define MADV_DONTNEED	0x4		/* discard these pages */
#elif defined(__sparc__) || defined (__powerpc__)
#define MAP_RENAME      MAP_ANONYMOUS   /* In SunOS terminology */
#define MAP_NORESERVE   0x40            /* don't reserve swap pages */
#define MAP_INHERIT     0x80            /* SunOS doesn't do this, but... */
#define MAP_LOCKED      0x100           /* lock the mapping */
#define _MAP_NEW        0x80000000      /* Binary compatibility is fun... */
#define MAP_GROWSDOWN	0x0100		/* stack-like segment */
#define MAP_DENYWRITE	0x0800		/* ETXTBSY */
#define MAP_EXECUTABLE	0x1000		/* mark it as an executable */
#define MS_ASYNC	1		/* sync memory asynchronously */
#define MS_INVALIDATE	2		/* invalidate the caches */
#define MS_SYNC		4		/* synchronous memory sync */
#define MCL_CURRENT     0x2000          /* lock all currently mapped pages */
#define MCL_FUTURE      0x4000          /* lock all additions to address space */
#define MADV_NORMAL	0x0		/* default page-in behavior */
#define MADV_RANDOM	0x1		/* page-in minimum required */
#define MADV_SEQUENTIAL	0x2		/* read-ahead aggressively */
#define MADV_WILLNEED	0x3		/* pre-fault pages */
#define MADV_DONTNEED	0x4		/* discard these pages */
#define MADV_FREE	0x5		/* (Solaris) contents can be freed */
#elif defined (__arm__)
#define MAP_GROWSDOWN	0x0100		/* stack-like segment */
#define MAP_DENYWRITE	0x0800		/* ETXTBSY */
#define MAP_EXECUTABLE	0x1000		/* mark it as an executable */
#define MAP_LOCKED	0x2000		/* pages are locked */
#define MAP_NORESERVE	0x4000		/* don't check for reservations */
#define MS_ASYNC	1		/* sync memory asynchronously */
#define MS_INVALIDATE	2		/* invalidate the caches */
#define MS_SYNC		4		/* synchronous memory sync */
#define MCL_CURRENT	1		/* lock all current mappings */
#define MCL_FUTURE	2		/* lock all future mappings */
#define MADV_NORMAL	0x0		/* default page-in behavior */
#define MADV_RANDOM	0x1		/* page-in minimum required */
#define MADV_SEQUENTIAL	0x2		/* read-ahead aggressively */
#define MADV_WILLNEED	0x3		/* pre-fault pages */
#define MADV_DONTNEED	0x4		/* discard these pages */

#elif defined(__hppa__)
#define MAP_TYPE    0x03        /* Mask for type of mapping */
#define MAP_FIXED   0x04        /* Interpret addr exactly */
#define MAP_ANONYMOUS   0x10        /* don't use a file */

#define MAP_DENYWRITE   0x0800      /* ETXTBSY */
#define MAP_EXECUTABLE  0x1000      /* mark it as an executable */
#define MAP_LOCKED  0x2000      /* pages are locked */
#define MAP_NORESERVE   0x4000      /* don't check for reservations */
#define MAP_GROWSDOWN   0x8000      /* stack-like segment */

#define MS_SYNC     1       /* synchronous memory sync */
#define MS_ASYNC    2       /* sync memory asynchronously */
#define MS_INVALIDATE   4       /* invalidate the caches */

#define MCL_CURRENT 1       /* lock all current mappings */
#define MCL_FUTURE  2

#endif

#endif

/* compatibility flags */
#define MAP_ANON	MAP_ANONYMOUS
#define MAP_FILE	0

#define MAP_FAILED      ((void *) -1)

extern void *mmap (void *__addr, size_t __len, int __prot,
                   int __flags, int __fd, off_t __offset);

extern int munmap (void *__addr, size_t __len) __THROW;
extern int mprotect (void *__addr, size_t __len, int __prot) __THROW;
extern int msync (void *__addr, size_t __len, int __flags) __THROW;
extern void *mremap (void *__addr, size_t __old_len, size_t __new_len,
		     unsigned long __may_move) __THROW;
extern int mincore (void *__start, size_t __len, unsigned char *__vec);

extern void *mmap64 (void *__addr, size_t __len, int __prot,
		     int __flags, int __fd, off64_t __offset) __THROW;

int mlockall(int flags) __THROW;
int mlock(const void *addr, size_t len) __THROW;
int munlockall(void) __THROW;

int madvise(void *start, size_t length, int advice) __THROW;

#define _POSIX_MAPPED_FILES

#endif
