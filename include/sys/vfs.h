#ifndef _SYS_VFS_H
#define _SYS_VFS_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <linux/vfs.h>

int statfs(const char *path, struct statfs *buf) __THROW;
int fstatfs(int fd, struct statfs *buf) __THROW;

#endif
