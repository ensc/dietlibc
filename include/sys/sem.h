#ifndef _SYS_SEM_H
#define _SYS_SEM_H

#include <linux/sem.h>
#include <sys/ipc.h>

extern int semget( key_t key, int nsems, int semflg) __THROW;
extern int semctl(int semid, int semnum, int cmd, union semun arg) __THROW;
extern int semop(int semid, struct sembuf *sops, unsigned nsops) __THROW;

#endif
