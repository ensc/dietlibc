#ifndef _SYS_MOUNT_H
#define _SYS_MOUNT_H

#include <sys/cdefs.h>
#include <sys/types.h>
#define _LINUX_CONFIG_H
#include <linux/fs.h>

int  mount(const char* specialfile, const char* dir, const char* filesystemtype,
	   unsigned long rwflag, const void * data) __THROW;

int umount(const char *specialfile) __THROW;

int umount(const char *dir) __THROW;

int umount2(const char *specialfile, int mflag) __THROW;

int umount2(const char *dir, int mflag) __THROW;

#endif
