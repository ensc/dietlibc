#include <string.h>
#include <netdb.h>

extern int getprotobynumber_r(int proto,
			   struct protoent *res, char *buf, size_t buflen,
			   struct protoent **res_sig) {
  int _res=-1;
  while (!getprotoent_r(res,buf,buflen,res_sig))
    if (proto==res->p_proto) { _res=0; break; }
  endprotoent();
  return _res;
}
