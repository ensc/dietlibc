#include <ctype.h>
#include <fnmatch.h>
#include <string.h>

#define NOTFIRST 128

static int match(char c,char d,int flags) {
  if (flags&FNM_CASEFOLD)
    return (tolower(c)==tolower(d));
  else
    return (c==d);
}

int fnmatch(const char *pattern, const char *string, int flags) {
  if (*string==0) {
    while (*pattern=='*') ++pattern;
    return (!!*pattern);
  }
  if (*string=='.' && *pattern!='.' && (flags&FNM_PERIOD)) {
    /* don't match if FNM_PERIOD and this is the first char */
    if (!(flags&NOTFIRST))
      return FNM_NOMATCH;
    /* don't match if FNM_PERIOD and FNM_PATHNAME and previous was '/' */
    if ((flags&(FNM_PATHNAME)) && string[-1]=='/')
      return FNM_NOMATCH;
  }
  flags|=NOTFIRST;
  switch (*pattern) {
  case '[':
    {
      int neg=0;
      const char* start;	/* first member of character class */

      ++pattern;
      if (*string=='/' && flags&FNM_PATHNAME) return FNM_NOMATCH;
      if (*pattern=='!') { neg=1; ++pattern; }
      start=pattern;
      while (*pattern) {
	int res=0;

	if (*pattern==']' && pattern!=start) break;
	if (*pattern=='[' && pattern[1]==':') {
	  /* stupid POSIX char classes */
	  char c=*string;

	  if (flags&FNM_CASEFOLD) c=tolower(c);
	  pattern+=2;
	  if (!strncmp(pattern,"alnum:]",7)) { if (isalnum(c)) res=1; pattern+=7; } else
	  if (!strncmp(pattern,"alpha:]",7)) { if (isalpha(c)) res=1; pattern+=7; } else
	  if (!strncmp(pattern,"blank:]",7)) { if (isblank(c)) res=1; pattern+=7; } else
	  if (!strncmp(pattern,"cntrl:]",7)) { if (iscntrl(c)) res=1; pattern+=7; } else
	  if (!strncmp(pattern,"digit:]",7)) { if (isdigit(c)) res=1; pattern+=7; } else
	  if (!strncmp(pattern,"graph:]",7)) { if (isgraph(c)) res=1; pattern+=7; } else
	  if (!strncmp(pattern,"lower:]",7)) { if (islower(c)) res=1; pattern+=7; } else
	  if (!strncmp(pattern,"print:]",7)) { if (isprint(c)) res=1; pattern+=7; } else
	  if (!strncmp(pattern,"punct:]",7)) { if (ispunct(c)) res=1; pattern+=7; } else
	  if (!strncmp(pattern,"space:]",7)) { if (isspace(c)) res=1; pattern+=7; } else
	  if (!strncmp(pattern,"upper:]",7)) {
	    if (flags&FNM_CASEFOLD?islower(c):isupper(c)) res=1; pattern+=7;
	  } else
	  if (!strncmp(pattern,"xdigit:]",8)) { if (isxdigit(c)) res=1; pattern+=8; } else
	    pattern-=2;
	}
	if (pattern[1]=='-') {
	  if (*string>=*pattern && *string<=pattern[2]) res=1;
	  if (flags&FNM_CASEFOLD) {
	    if (tolower(*string)>=tolower(*pattern) && tolower(*string)<=tolower(pattern[2])) res=1;
	  }
	  pattern+=3;
	} else {
	  res=match(*pattern,*string,flags);
	  ++pattern;
	}
	if (res ^ neg) {
	  while (*pattern && *pattern!=']') ++pattern;
	  return fnmatch(pattern+1,string+1,flags);
	}
      }
    }
    break;
  case '\\':
    if (flags&FNM_NOESCAPE) {
      if (*string=='\\')
	return fnmatch(pattern+1,string+1,flags);
    } else {
      if (*string==pattern[1])
	return fnmatch(pattern+2,string+1,flags);
    }
    break;
  case '*':
    if ((*string=='/' && flags&FNM_PATHNAME) || fnmatch(pattern,string+1,flags))
      return fnmatch(pattern+1,string,flags);
    return 0;
  case 0:
    if (*string==0 || (*string=='/' && (flags&FNM_LEADING_DIR)))
      return 0;
    break;
  case '?':
    if (*string=='/' && flags&FNM_PATHNAME) break;
    return fnmatch(pattern+1,string+1,flags);
  default:
    if (match(*pattern,*string,flags))
      return fnmatch(pattern+1,string+1,flags);
    break;
  }
  return FNM_NOMATCH;
}
