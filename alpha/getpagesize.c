#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

size_t getpagesize(void) {
  return PAGE_SIZE;
}
