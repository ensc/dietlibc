#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <asm/ipc.h>

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif

size_t getpagesize(void) {
  return PAGE_SIZE;
}
