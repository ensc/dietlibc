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
  /*printf("fnmatch(\"%s\",\"%s\")\n",pattern,string);*/
  if (*string==0) {
    while (*pattern=='*') ++pattern;
    return (!!*pattern);
  }
  if (*string=='.' && *pattern!='.' && (flags&FNM_PERIOD)) {
    /* don't match if FNM_PERIOD and this is the first char */
    if ((flags&FNM_PERIOD) && (!(flags&NOTFIRST)))
      return FNM_NOMATCH;
    /* don't match if FNM_PERIOD and FNM_FILE_NAME and previous was '/' */
    if ((flags&(FNM_FILE_NAME|FNM_PERIOD)) && string[-1]=='/')
      return FNM_NOMATCH;
  }
  flags|=NOTFIRST;
  switch (*pattern) {
  case '[':
    {
      int neg=0;
      ++pattern;
      if (*string=='/' && flags&FNM_PATHNAME) return FNM_NOMATCH;
      if (*pattern=='!') { neg=1; ++pattern; }
      while (*pattern && *pattern!=']') {
	int res=0;
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
  default:
    if (match(*pattern,*string,flags))
      return fnmatch(pattern+1,string+1,flags);
    break;
  }
  return FNM_NOMATCH;
}
