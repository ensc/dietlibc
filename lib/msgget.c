#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <asm/ipc.h>

extern int __ipc();

int msgget(key_t key,int flag) {
  return __ipc(MSGGET,key,flag,0,0);
}
