#include <pwd.h>
#include <string.h>
#include <stdlib.h>

struct passwd *getpwuid(uid_t uid) {
  struct passwd *tmp;
  setpwent();
  for (;;) {
    tmp=getpwent();
    if (tmp->pw_uid==uid)
      return tmp;
    free(tmp);
  }
  return 0;
}
