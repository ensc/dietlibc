#ifndef _DIRENT_H
#define _DIRENT_H       1

#include <sys/cdefs.h>
#include <sys/types.h>

#include <linux/dirent.h>

#if 0
struct dirent
  {
#ifndef __USE_FILE_OFFSET64
    ino_t d_ino;
    off_t d_off;
#else
    ino64_t d_ino;
    off64_t d_off;
#endif
    unsigned short int d_reclen;
    unsigned char d_type;
    char d_name[256];		/* We must not include limits.h! */
  };

#ifdef __USE_LARGEFILE64
struct dirent64
  {
    ino64_t d_ino;
    off64_t d_off;
    unsigned short int d_reclen;
    unsigned char d_type;
    char d_name[256];		/* We must not include limits.h! */
  };
#endif
#endif

#define d_fileno	d_ino	/* Backwards compatibility.  */

#undef  _DIRENT_HAVE_D_NAMLEN
#define _DIRENT_HAVE_D_RECLEN
#define _DIRENT_HAVE_D_OFF
#define _DIRENT_HAVE_D_TYPE

#define opendir(name) __dietopendir(name)
#define closedir(dir) __dietclosedir(dir)
#define readdir(dir) __dietreaddir(dir)
#define readdir64(dir) __dietreaddir64(dir)
#define rewinddir(dir) __dietrewinddir64(dir)
#define seekdir(dir,pos) __dietseekdir(dir,pos)
#define telldir(dir) __diettelldir(dir)

typedef struct __dirstream DIR;

DIR *opendir (const char *__name) __THROW;
int closedir (DIR *__dirp) __THROW;
struct dirent *readdir (DIR *__dirp) __THROW;
struct dirent64 *readdir64 (DIR *__dirp) __THROW;
void rewinddir (DIR *__dirp) __THROW;
void seekdir (DIR *__dirp, long int __pos) __THROW;
long int telldir (DIR *__dirp) __THROW;

#endif
