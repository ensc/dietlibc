#include <dietfeatures.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <syscalls.h>
#include <errno.h>

#include "../dietpagesize.h"

#ifdef __NR_mmap2
void*__mmap2(void*start,size_t length,int prot,int flags,int fd,off_t pgoffset);

void*__libc_mmap64(void*addr,size_t len,int prot,int flags,int fd,off64_t offset);
void*__libc_mmap64(void*addr,size_t len,int prot,int flags,int fd,off64_t offset) {
  if (offset&(__DIET_PAGE_SIZE)) {
    errno=-EINVAL;
    return MAP_FAILED;
  }
  return __mmap2(addr,len,prot,flags,fd,offset>>__DIET_PAGE_SHIFT);
}

void*mmap64(void*addr,size_t len,int prot,int flags,int fd,off64_t offset)
__attribute__((weak,alias("__libc_mmap64")));
#endif
