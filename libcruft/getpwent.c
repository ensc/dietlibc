#include <pwd.h>

extern struct passwd __passwd_pw;
extern char __passwd_buf[1000];

struct passwd *getpwent(void) {
  struct passwd* tmp;
  if (getpwent_r(&__passwd_pw,__passwd_buf,sizeof(__passwd_buf),&tmp)==0)
    return tmp;
  return 0;
}
