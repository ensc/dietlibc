#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <asm/ipc.h>

extern int __ipc();

int shmget(key_t key, int size, int shmflg) {
  return __ipc(SHMGET,key,size,shmflg,0);
}
