#include <pwd.h>
#include <string.h>
#include <stdlib.h>

struct passwd *getpwnam(const char * name) {
  struct passwd *tmp;
  setpwent();
  for (;;) {
    tmp=getpwent();
    if (!tmp) return 0;
    if (!strcmp(tmp->pw_name,name))
      return tmp;
  }
}
