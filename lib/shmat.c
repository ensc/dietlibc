#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <asm/ipc.h>

extern void* __ipc();

void* shmat(int shmid,const void* shmaddr,int shmflg) {
  void* raddr;
  register void* result;
  result=__ipc(SHMAT,shmid,shmflg,&raddr,shmaddr);
#ifdef __sparc__
  if ((unsigned long)result <= -(unsigned long)PAGE_SIZE)
#else
  if ((unsigned long)result <= -(unsigned long)SHMLBA)
#endif
    result=raddr;
  return result;
}
