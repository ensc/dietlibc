#ifndef _SYS_MOUNT_H
#define _SYS_MOUNT_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <linux/fs.h>

int  mount(const char* specialfile, const char* dir, const char* filesystemtype,
	   unsigned long rwflag, const void * data) __THROW;

int umount(const char *specialfile) __THROW;

int umount(const char *dir) __THROW;


#endif
