#include <string.h>

void explicit_bzero(void* dest,size_t len) {
  memset(dest,0,len);
  asm volatile("": : "r"(dest) : "memory");
}
