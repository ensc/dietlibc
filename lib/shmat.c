#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <asm/ipc.h>

extern int __ipc();

void* shmat(int shmid,const void* shmaddr,int shmflg) {
  void* raddr;
  register int result;
  result=__ipc(SHMAT,shmid,shmflg,&raddr,shmaddr);
  if ((unsigned long)result <= -(unsigned long)SHMLBA)
    result=raddr;
  return result;
}
