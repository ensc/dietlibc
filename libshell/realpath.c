/* realpath - determine the absolute pathname of a file

   Copyright (C) 2003 Thomas M. Ogrisegg <tom-dietlibc@fnord.at>

   This realpath has still space for improvements:
   e.g., if the initial path does not start with '/' it should
   be checked whether the first path component(s) is/are symlinks.
   This would save lots of filesystem lookups, but also greatly
   increases complexity.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define CD(x) (x=='/'||!x)

char *
realpath (const char *file, char *dst)
{
  unsigned char buffer[PATH_MAX];
  size_t i, off;

  if (!file || !dst) { errno = EINVAL; return (NULL); }
  if (!*file) { errno = ENOENT; return (NULL); }

  if (*file != '/') {
    if (!getcwd (buffer, sizeof (buffer))) return (NULL);
    off = strlen (buffer);
    while (file[0] == '.' && file[1] == '.' && file[2] == '/') {
      if (off) off--;
      while (off && buffer[off] != '/') off--;
      file += 3;
    }
    buffer[off++] = '/';
    i = strlen (file);
    if (i+off >= PATH_MAX) return (NULL);
    memcpy (buffer+off, file, i+1);
    file = buffer;
  }

  off = i = 0;

  while (file[i]) {
    if (file[i] == '.' && file[i-1] == '/' &&
       ((file[i+1] == '.'&&CD(file[i+2])) || CD(file[i+1]))) {
      if (file[i+1] == '/') { i+=2; continue; }
      if (file[i+1] == '.') {
        if (off > 2) off -= 2;
        while (off && dst[off] != '/') off--;
        i++;
      }
      i++;
      continue;
    }

    if (file[i] == '/' && off < PATH_MAX) {
      unsigned char buf[PATH_MAX];
      int ret;

      if (file[i+1] == '/') { i+=2; continue; }

      if (off) {
        if (dst[off-1] == '/') { i++; continue; }

last_test:
        dst[off] = 0;
        if (-1 != (ret = readlink (dst, buf, sizeof (buf)))) {
          struct stat st;
          buf[ret] = 0;
          if (-1 == stat (buf, &st)) return (NULL);
          if (ret+(PATH_MAX-i) > PATH_MAX) return (NULL);
          strcpy (buf+ret, file+i);
          return (realpath (buf, dst));
        } else
          if (errno == EACCES || errno == ENOENT) return (NULL);

        if (!file[i]) return (dst);
      }
    }
    dst[off++] = file[i++];
  }

  if (off > 2 && dst[off-1] == '/') off--;
  goto last_test;
}
