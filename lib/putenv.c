#include <stdlib.h>
#include <string.h>
#include <errno.h>

int putenv(const char *string) {
  size_t len;
  int envc;
  char *tmp;
  const char **ep;
  char **newenv;
  static char **origenv=0;
  if (!origenv) origenv=environ;
  if (!(tmp=strchr(string,'='))) {
    errno=EINVAL;
    return 0;
  }
  len=tmp-string+1;
  for (envc=0, ep=(const char**)environ; *ep; ++ep) {
    if (!memcmp(string,*ep,len) && ep[0][len]=='=') { /* found */
      if (!tmp) {
	for (; ep[1]; ++ep) ep[0]=ep[1];
	ep[0]=0;
	return 0;
      }
      *ep=string;
      return 0;
    }
    ++envc;
  }
  if (tmp) {
    newenv=(char**)malloc((envc+2)*sizeof(char*));
    if (!newenv) return -1;
    newenv[0]=(char*)string;
    memcpy(newenv+1,environ,(envc+1)*sizeof(char*));
    if (environ!=origenv) free(environ);
    environ=newenv;
  }
  return 0;
}
