#ifndef	_CTYPE_H
#define _CTYPE_H

#include <sys/cdefs.h>

extern int isascii (int c) __THROW __attribute__ ((__const__));
extern int isblank (int c) __THROW __attribute__ ((__const__));
extern int isalnum (int c) __THROW __attribute__ ((__const__));
extern int isalpha (int c) __THROW __attribute__ ((__const__));
extern int isdigit (int c) __THROW __attribute__ ((__const__));
extern int isspace (int c) __THROW __attribute__ ((__const__));

extern int isupper (int c) __THROW __attribute__ ((__const__));
extern int islower (int c) __THROW __attribute__ ((__const__));

extern int tolower(int c) __THROW __attribute__ ((__const__));
extern int toupper(int c) __THROW __attribute__ ((__const__));

#endif
