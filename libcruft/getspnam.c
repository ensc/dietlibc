#include <shadow.h>
#include <string.h>

extern struct spwd __shadow_pw;
extern char __shadow_buf[1000];

struct spwd *getspnam(const char* name) {
  struct spwd *tmp;
  if (getspnam_r(name,&__shadow_pw,__shadow_buf,sizeof(__shadow_buf),&tmp)==0)
    return tmp;
  return 0;
}
