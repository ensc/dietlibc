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
    if ((flags&FNM_PERIOD) && (!(flags&NOTFIRST)))
      return 1;
    /* don't match if FNM_PERIOD and FNM_FILE_NAME and previous was '/' */
    if ((flags&(FNM_FILE_NAME|FNM_PERIOD)) && string[-1]=='/')
      return 1;
  }
  flags|=NOTFIRST;
  switch (*pattern) {
  case '[':
    ++pattern;
    while (*pattern && *pattern!=']') {
      if (flags&FNM_PATHNAME || *string!='/') {
	int res=0;
	if (pattern[1]=='-') {
	  if (*string>=*pattern && *string<=pattern[2]) res=1;
	  if (flags&FNM_CASEFOLD) {
	    if (tolower(*string)>=tolower(*pattern) && tolower(*string)<=tolower(pattern[2])) res=1;
	  }
	} else
	  res=match(*pattern,*string,flags);
	if (res) {
	  while (*pattern && *pattern!=']') ++pattern;
	  return fnmatch(pattern+1,string+1,flags);
	}
      }
      ++pattern;
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
    if (!pattern[1] || fnmatch(pattern+1,string,flags)==0) {
      if ((flags&FNM_PATHNAME) && strchr(string,'/'))
	return FNM_NOMATCH;
      return 0;
    } else
      if (*string!='/')
	return fnmatch(pattern,string+1,flags);
    break;
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
