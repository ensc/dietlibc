#ifndef _STRINGS_H
#define _STRINGS_H

#include <stddef.h>
#include <sys/cdefs.h>

int strcasecmp(const char *s1, const char *s2) __THROW __pure__;
int strncasecmp(const char *s1, const char *s2, size_t n) __THROW __pure__;
int ffs(int i) __THROW __attribute__((__const__));

int    bcmp(const void *, const void *, size_t) __THROW __pure__;
void   bcopy(const void *, void *, size_t) __THROW;
void   bzero(void *, size_t) __THROW;
char  *index(const char *, int) __THROW __pure__;
char  *rindex(const char *, int) __THROW __pure__;

#define bzero(s,n) memset(s,0,n)
#define bcopy(src,dest,n) memmove(dest,src,n)
#define bcmp(a,b,n) memcmp(a,b,n)
#define index(a,b) strchr(a,b)
#define rindex(a,b) strrchr(a,b)

#endif
