#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <asm/ipc.h>

extern int __ipc();

int shmdt(const void* shmaddr) {
  return __ipc(SHMDT,0,0,0,shmaddr);
}
