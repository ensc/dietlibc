#include <string.h>
#include <grp.h>

int getgrnam_r(const char* name,
	       struct group *res, char *buf, size_t buflen,
	       struct group **res_sig) {
  int _res=-1;
  while (!getgrent_r(res,buf,buflen,res_sig)) {
    if (!strcmp(name,res->gr_name)) {
      _res=0;
      break;
    }
  }
  endgrent();
  return _res;
}
