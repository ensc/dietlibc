#include <string.h>
#include <shadow.h>

int getspnam_r(const char* name,
	       struct spwd *res, char *buf, size_t buflen,
	       struct spwd **res_sig) {
  int _res=-1;
  while (!getspent_r(res,buf,buflen,res_sig)) {
    if (!strcmp(name,res->sp_namp)) {
      _res=0;
      break;
    }
  }
  endspent();
  return _res;
}
