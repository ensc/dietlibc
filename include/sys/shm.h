#ifndef _SYS_SHM_H
#define _SYS_SHM_H

#include <linux/shm.h>
#include <sys/ipc.h>
#include <asm/page.h>

#ifndef PAGE_SIZE
#define PAGE_SIZE getpagesize()
#endif

extern int shmget(key_t key, int size, int shmflg) __THROW;
extern void *shmat(int shmid, const void *shmaddr, int shmflg) __THROW;
extern int shmdt (const void *shmaddr) __THROW;
extern int shmctl(int shmid, int cmd, struct shmid_ds *buf) __THROW;

#endif
