#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <asm/ipc.h>

extern int __ipc();

int semget(key_t key, int nsems, int semflg) {
  return __ipc(SEMGET,key,nsems,semflg,0);
}
