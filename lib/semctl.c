#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

extern int __ipc(int,int,int,int,void*);

int semctl(int semid, int semnum, int cmd, union semun arg) {
  return __ipc(SEMCTL,semid,semnum,cmd,&arg);
}
