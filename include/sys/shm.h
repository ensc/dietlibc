#ifndef _SYS_SHM_H
#define _SYS_SHM_H

#include <linux/shm.h>
#include <sys/ipc.h>

extern int shmget(key_t key, int size, int shmflg) __THROW;
extern void *shmat(int shmid, const void *shmaddr, int shmflg) __THROW;
extern void int shmdt (const void *shmaddr) __THROW;

#endif
