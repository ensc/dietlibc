#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <asm/ipc.h>

extern int __ipc();

int semop(int semid,struct sembuf *sops,unsigned nsops) {
  return __ipc(SEMOP,semid,nsops,0,sops);
}
