#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <asm/ipc.h>

extern int __ipc();

int msgrcv(int msqid, void *msgp, size_t msgsz, long int msgtyp, int msgflg) {
  struct ipc_kludge tmp;
  tmp.msgp = msgp;
  tmp.msgtyp = msgtyp;
  return __ipc(MSGRCV,msqid, msgsz, msgflg, &tmp);
}
