#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <asm/ipc.h>

extern int __ipc();

int msgctl(int msqid, int cmd, struct msqid_ds *buf) {
  return __ipc(MSGCTL,msqid,cmd,0,buf);
}
