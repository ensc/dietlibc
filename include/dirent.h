#ifndef _DIRENT_H
#define _DIRENT_H       1

#include <sys/cdefs.h>
#include <sys/types.h>
#include <limits.h>

struct dirent {
  long		d_ino;
  off_t		d_off;
  unsigned short	d_reclen;
  char		d_name[256]; /* We must not include limits.h! */
};

#ifndef __STRICT_ANSI__
struct dirent64 {
  uint64_t	d_ino;
  int64_t	d_off;
  unsigned short	d_reclen;
  unsigned char	d_type;
  char		d_name[256];
};
#endif

#define d_fileno	d_ino	/* Backwards compatibility.  */

#undef  _DIRENT_HAVE_D_NAMLEN
#define _DIRENT_HAVE_D_RECLEN
#define _DIRENT_HAVE_D_OFF

typedef struct __dirstream DIR;

DIR *opendir (const char *__name) __THROW;
int closedir (DIR *__dirp) __THROW;
struct dirent *readdir (DIR *__dirp) __THROW;
struct dirent64 *readdir64 (DIR *__dirp) __THROW;
void rewinddir (DIR *__dirp) __THROW;
void seekdir (DIR *__dirp, long int __pos) __THROW;
long int telldir (DIR *__dirp) __THROW;

int scandir(const char *dir, struct dirent ***namelist,
      int (*select)(const struct dirent *),
      int (*compar)(const struct dirent **, const struct dirent **)) __THROW;

int alphasort(const struct dirent **a, const struct dirent **b) __THROW __attribute__((const));

#define MAXNAMLEN NAME_MAX

#ifdef _BSD_SOURCE
extern int dirfd(DIR *dirp) __THROW;
#endif

#if defined _FILE_OFFSET_BITS && _FILE_OFFSET_BITS == 64
#define dirent dirent64
#define readdir readdir64
#endif

#endif
