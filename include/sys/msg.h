#ifndef _SYS_MSG_H
#define _SYS_MSG_H

#include <linux/msg.h>
#include <sys/ipc.h>
#include <asm/page.h>

extern int msgctl (int msqid, int cmd, struct msqid_ds *buf) __THROW;
extern int msgget (key_t key, int msgflg) __THROW;
extern int msgrcv (int msqid, void *msgp, size_t msgsz, long int msgtyp, int msgflg) __THROW;
extern int msgsnd (int msqid, const void *msgp, size_t msgsz, int msgflg) __THROW;

#endif
