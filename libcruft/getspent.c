#include <shadow.h>

extern struct spwd __shadow_pw;
extern char __shadow_buf[1000];

struct spwd *getspent(void) {
  struct spwd* tmp;
  if (getspent_r(&__shadow_pw,__shadow_buf,sizeof(__shadow_buf),&tmp)==0)
    return tmp;
  return 0;
}
