#include <string.h>
#include <pwd.h>

int getpwnam_r(const char* name,
	       struct passwd *res, char *buf, size_t buflen,
	       struct passwd **res_sig) {
  int _res=-1;
  while (!getpwent_r(res,buf,buflen,res_sig)) {
    if (!strcmp(name,res->pw_name)) {
      _res=0;
      break;
    }
  }
  endpwent();
  return _res;
}
