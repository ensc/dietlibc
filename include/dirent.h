#ifndef _DIRENT_H
#define _DIRENT_H       1

#include <sys/cdefs.h>
#include <sys/types.h>

#include <linux/dirent.h>

#define d_fileno	d_ino	/* Backwards compatibility.  */

#undef  _DIRENT_HAVE_D_NAMLEN
#define _DIRENT_HAVE_D_RECLEN
#define _DIRENT_HAVE_D_OFF
#define _DIRENT_HAVE_D_TYPE

typedef struct __dirstream DIR;

DIR *opendir (const char *__name) __THROW;
int closedir (DIR *__dirp) __THROW;
struct dirent *readdir (DIR *__dirp) __THROW;
struct dirent64 *readdir64 (DIR *__dirp) __THROW;
void rewinddir (DIR *__dirp) __THROW;
void seekdir (DIR *__dirp, long int __pos) __THROW;
long int telldir (DIR *__dirp) __THROW;

#endif
