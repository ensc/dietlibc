#include <pwd.h>

int getpwuid_r(uid_t uid,
	       struct passwd *res, char *buf, size_t buflen,
	       struct passwd **res_sig) {
  int _res=-1;
  while (!getpwent_r(res,buf,buflen,res_sig)) {
    if (uid==res->pw_uid) {
      _res=0;
      break;
    }
  }
  endpwent();
  return _res;
}
