#include "_dl_int.h"

void *dlsym(void *handle, char *symbol)
{
  printf("dlsym(%08x,%s) -> %08x\n",handle,symbol,elf_hash(symbol)%17);
  return 0;
}
