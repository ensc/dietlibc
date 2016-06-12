/* fast strcpy -- Copyright (C) 2003 Thomas M. Ogrisegg <tom@hi-tek.fnord.at> */
#include <string.h>
#include "dietfeatures.h"
#include "dietstring.h"

#ifdef WANT_VALGRIND_SUPPORT
extern int __valgrind;
#endif

char *
strcpy (char *s1, const char *s2)
{
    char           *res = s1;
#ifdef WANT_SMALL_STRING_ROUTINES
    while ((*s1++ = *s2++));
    return (res);
#else
    int             tmp;
    unsigned long   l;

    if (UNALIGNED(s1, s2)
#ifdef WANT_VALGRIND_SUPPORT
        || __unlikely(__valgrind)
#endif
    ) {
	while ((*s1++ = *s2++));
	return (res);
    }
    if ((tmp = STRALIGN(s1))) {
	while (tmp-- && (*s1++ = *s2++));
	if (tmp != -1) return (res);
    }

    while (1) {
	l = *(const unsigned long *) s2;
	if (((l - MKW(0x1ul)) & ~l) & MKW(0x80ul)) {
	    while ((*s1++ = GFC(l))) INCSTR(l);
	    return (res);
	}
	*(unsigned long *) s1 = l;
	s2 += sizeof(unsigned long);
	s1 += sizeof(unsigned long);
    }
#endif
}
