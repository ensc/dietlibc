#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "dietfeatures.h"

int execle( const char *path, const char* arg, ...) {
  va_list ap;
  int n,i;
  char **argv,*tmp, **env;
  va_start(ap, arg);
  (void)arg;
  n=2;
  while ((tmp=va_arg(ap,char *)))
    ++n;
  va_end (ap);
  if ((argv=(char **)alloca(n*sizeof(char*)))) {
    va_start(ap, arg);
    argv[0]=(char*)arg;
    for (i=1; i<n; ++i)
      argv[i]=va_arg(ap,char *);
    env=va_arg(ap, char **);
    va_end (ap);
    return execve(path,argv,env);
  }
  errno=ENOMEM;
  return -1;
}
